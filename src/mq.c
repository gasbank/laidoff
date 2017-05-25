#include <czmq.h>
#include <stdlib.h>
#include "mq.h"
#include "lwlog.h"
#include "sysmsg.h"
#include "kvmsg.h"
#include "field.h"
#include "lwcontext.h"
#include "extrapolator.h"

//  Mutex macros
#if defined (__UNIX__)
typedef pthread_mutex_t zsys_mutex_t;
#   define ZMUTEX_INIT(m)    pthread_mutex_init (&m, NULL);
#   define ZMUTEX_LOCK(m)    pthread_mutex_lock (&m);
#   define ZMUTEX_UNLOCK(m)  pthread_mutex_unlock (&m);
#   define ZMUTEX_DESTROY(m) pthread_mutex_destroy (&m);
#elif defined (__WINDOWS__)
typedef CRITICAL_SECTION zsys_mutex_t;
#   define ZMUTEX_INIT(m)    InitializeCriticalSection (&m);
#   define ZMUTEX_LOCK(m)    EnterCriticalSection (&m);
#   define ZMUTEX_UNLOCK(m)  LeaveCriticalSection (&m);
#   define ZMUTEX_DESTROY(m) DeleteCriticalSection (&m);
#endif

typedef enum _LW_MESSAGE_QUEUE_STATE {
	LMQS_INIT,
	LMQS_TIME,
	LMQS_SNAPSHOT,
	LMQS_READY,
	LMQS_TERM,
} LW_MESSAGE_QUEUE_STATE;

typedef struct _LWMESSAGEQUEUE {
	zhash_t* kvmap;
	zhash_t* posmap; // UUID --> remote entity position extrapolator
	int64_t sequence;
	zsock_t* snapshot;
	zsock_t* subscriber;
	zsock_t* publisher;
	int port;
	char* subtree;
	LW_MESSAGE_QUEUE_STATE state;
	int64_t alarm;
	zuuid_t* uuid;
	int verbose;
	double delta;
	double* deltasamples;
	int deltasequence;
	int64_t start_req_time_mono;
	zsys_mutex_t mutex;
} LWMESSAGEQUEUE;

#define DELTA_REQ_COUNT (15)
#define SUBTREE "/l/"

static void s_req_time(LWMESSAGEQUEUE* mq);

void* init_mq(const char* addr, void* sm) {
	zsys_init();

	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)calloc(1, sizeof(LWMESSAGEQUEUE));
	mq->state = LMQS_INIT;
	mq->subtree = SUBTREE;
	mq->port = 5556;
	mq->verbose = 0;
	// Prepare our context and subscriber
	mq->snapshot = zsock_new(ZMQ_DEALER);
	zsock_connect(mq->snapshot, "tcp://%s:%d", addr, mq->port);
	mq->subscriber = zsock_new(ZMQ_SUB);
	zsock_set_subscribe(mq->subscriber, mq->subtree);
	zsock_connect(mq->subscriber, "tcp://%s:%d", addr, mq->port + 1);
	mq->publisher = zsock_new(ZMQ_PUSH);
	zsock_connect(mq->publisher, "tcp://%s:%d", addr, mq->port + 2);

	mq->kvmap = zhash_new();
	mq->posmap = zhash_new();
	mq->sequence = 0;

	mq->uuid = zuuid_new();
	mq->deltasequence = 0;
	mq->delta = 0;
	mq->deltasamples = (double*)malloc(sizeof(double) * DELTA_REQ_COUNT);

	char sys_msg[128];
	sprintf(sys_msg, LWU("Connecting %s"), addr);
	show_sys_msg(sm, sys_msg);

	// First we request a time sync:
	mq->start_req_time_mono = zclock_mono();
	s_req_time(mq);

	ZMUTEX_INIT(mq->mutex);

	return mq;
}

static void s_req_time(LWMESSAGEQUEUE* mq) {
	zstr_sendm(mq->snapshot, "WHTTAIM?");
	char clienttime[64];
	sprintf(clienttime, "%f", mq_mono_clock());
	zstr_send(mq->snapshot, clienttime);
	mq->state = LMQS_TIME;
}

static void
s_kvmsg_free_posmap(void* ptr) {
	if (ptr) {
		LWPOSSYNCMSG* possyncmsg = (LWPOSSYNCMSG*)ptr;
		vec4_extrapolator_destroy(&possyncmsg->extrapolator);
		free(ptr);
	}
}

static void
s_kvmsg_store_posmap_noown(kvmsg_t** self_p, zhash_t* hash, double sync_time, LWMESSAGEQUEUE* mq) {
	assert(self_p);
	if (*self_p) {
		kvmsg_t* self = *self_p;
		assert(self);
		// kvmsg_size(self) would be 0 if repeat_last is 1.
		// Any of these is nonzero, we process the message.
		if (kvmsg_size(self)) {
			// kvmsg_body may return unaligned memory address
			// which may signal SIGBUS error on Android when
			// accessing struct member variables.
			// We workaround by copying it to aligned memory address.
			LWMQMSG* msg_unaligned = (LWMQMSG*)kvmsg_body(self);
			LWMQMSG msg_aligned;
			// Should not use 'memcpy'. It is not supported on unaligned addresses.
			for (int i = 0; i < sizeof(LWMQMSG); i++) {
				((char*)&msg_aligned)[i] = ((char*)msg_unaligned)[i];
			}
			LWMQMSG* msg = &msg_aligned;
			LWPOSSYNCMSG* possyncmsg = zhash_lookup(hash, kvmsg_key(self));
			if (possyncmsg) {
				// Do nothing
			} else {
				// Create a new extrapolator
				possyncmsg = (LWPOSSYNCMSG*)malloc(sizeof(LWPOSSYNCMSG));
				possyncmsg->a = 0;
				possyncmsg->extrapolator = vec4_extrapolator_new();
				vec4_extrapolator_reset(possyncmsg->extrapolator, LWMIN(msg->t, sync_time) /* avoid assertion in extrapolator*/,
					sync_time, msg->x, msg->y, msg->z, msg->dx, msg->dy);
				zhash_update(hash, kvmsg_key(self), possyncmsg);
				zhash_freefn(hash, kvmsg_key(self), s_kvmsg_free_posmap);
				if (mq->verbose) {
					LOGI("New possyncmsg entry with key %s created.", kvmsg_key(self));
				}
			}

			if (mq->verbose && !mq_cursor_player(mq, kvmsg_key(self))) {
				LOGI("UPDATE: t %.2f POS (%.2f, %.2f, %.2f) DXY (%.2f, %.2f)", msg->t, msg->x,
					msg->y, msg->z, msg->dx, msg->dy);
			}

			//possyncmsg->attacking = msg->attacking;
			//possyncmsg->moving = msg->moving;
			possyncmsg->action = msg->action;
			if (msg->stop) {
				vec4_extrapolator_add_stop(possyncmsg->extrapolator, msg->t, sync_time, msg->x,
					msg->y, msg->z, msg->dx, msg->dy);
			} else {
				vec4_extrapolator_add(possyncmsg->extrapolator, msg->t, sync_time, msg->x,
					msg->y, msg->z, msg->dx, msg->dy);
			}
			//LOGI("New possyncmsg entry with key %s updated.", kvmsg_key(self));

		} else {
			zhash_delete(hash, kvmsg_key(self));
		}
	}
}

static int s_delta_cmp(const void* a, const void* b) {
	double d = *(double*)a - *(double*)b;
	return d > 0 ? 1 : d < 0 ? -1 : 0;
}

static void s_mq_poll_time(void* _mq, void* sm) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	zmq_pollitem_t items[] = { { zsock_resolve(mq->snapshot), 0, ZMQ_POLLIN, 0 } };
	int rc = zmq_poll(items, 1, 0);
	if (rc == -1) {
		return;
	}

	if (items[0].revents & ZMQ_POLLIN) {
		char* name = zstr_recv(mq->snapshot);
		if (name) {
			free(name);
			char* timereply = zstr_recv(mq->snapshot);
			if (timereply) {
				// https://en.wikipedia.org/wiki/Network_Time_Protocol#Clock_synchronization_algorithm
				double t2 = 0, t1 = 0, t0 = 0;
				sscanf(timereply, "%lf %lf %lf", &t2, &t1, &t0);
				double t3 = mq_mono_clock();
				double delta = ((t1 - t0) + (t2 - t3)) / 2;
				mq->deltasamples[mq->deltasequence++] = delta;
				double roundtrip_delay = (t3 - t0) - (t2 - t1);

				/*if (mq->verbose)*/
				{
					LOGI("REQ %d: delta = %f sec, rtd = %f sec", mq->deltasequence, delta, roundtrip_delay);
				}

				char sys_msg[128];
				sprintf(sys_msg, LWU("Timesyncing %d/%d"), mq->deltasequence, DELTA_REQ_COUNT);
				show_sys_msg(sm, sys_msg);
				// Request for another or finish
				if (mq->deltasequence < DELTA_REQ_COUNT) {
					s_req_time(mq);
				} else {
					int64_t req_time_elapsed = zclock_mono() - mq->start_req_time_mono;
					LOGI("Tymesyncing duration: %f sec (%f sec / req)", req_time_elapsed / 1e3, req_time_elapsed / 1e3 / DELTA_REQ_COUNT);

					// Calculate averaged delta around a median delta:
					qsort(mq->deltasamples, DELTA_REQ_COUNT, sizeof(mq->deltasamples[0]), s_delta_cmp);
					int nselsamples = 10;
					int ibeg = LWMAX(0, DELTA_REQ_COUNT / 2 - nselsamples / 2);
					int iend = LWMIN(DELTA_REQ_COUNT / 2 + nselsamples / 2, DELTA_REQ_COUNT);
					double selsum = 0;
					for (int i = ibeg; i < iend; i++) {
						selsum += mq->deltasamples[i];
					}
					mq->delta = selsum / nselsamples;
					LOGI("FINAL delta = %f", mq->delta);

					// Next, we request a state snapshot:
					zstr_sendm(mq->snapshot, "ICANHAZ?");
					zstr_send(mq->snapshot, mq->subtree);
					mq->state = LMQS_SNAPSHOT;
				}
			}
		}
	}
}

static void s_mq_poll_snapshot(void* _mq, void* sm) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	zmq_pollitem_t items[] = { { zsock_resolve(mq->snapshot), 0, ZMQ_POLLIN, 0 } };
	int rc = zmq_poll(items, 1, 0);
	if (rc == -1) {
		return;
	}

	if (items[0].revents & ZMQ_POLLIN) {
		kvmsg_t* kvmsg = kvmsg_recv(zsock_resolve(mq->snapshot));
		if (!kvmsg) {
			return;
		}
		if (streq(kvmsg_key(kvmsg), "KTHXBAI")) {
			mq->sequence = kvmsg_sequence(kvmsg);
			LOGI("received snapshot=%"PRId64, mq->sequence);
			kvmsg_destroy(&kvmsg);
			mq->state = LMQS_READY;
			mq->alarm = zclock_time() + 100;
			show_sys_msg(sm, LWU("Snapshot downloaded"));
			return;
		}
		show_sys_msg(sm, LWU("Receiving snapshot"));
		mq_lock_mutex(mq);
		s_kvmsg_store_posmap_noown(&kvmsg, mq->posmap, mq_sync_mono_clock(mq), mq);
		mq_unlock_mutex(mq);
		kvmsg_store(&kvmsg, mq->kvmap);
	}
}

static void s_send_pos(const LWCONTEXT* pLwc, LWMESSAGEQUEUE* mq, int stop) {
	kvmsg_t* kvmsg = kvmsg_new(0);
	kvmsg_fmt_key(kvmsg, "%s%s", mq->subtree, zuuid_str(mq->uuid));
	LWMQMSG msg;
	memset(&msg, 0, sizeof(LWMQMSG));
	get_field_player_position(pLwc->field, &msg.x, &msg.y, &msg.z);
	msg.dx = pLwc->player_pos_last_moved_dx;
	msg.dy = pLwc->player_pos_last_moved_dy;
	//msg.moving = pLwc->player_moving;
	//msg.attacking = pLwc->player_attacking;
	msg.stop = stop;
	msg.action = get_anim_by_state(pLwc->player_state_data.state, 0);
	msg.t = mq_sync_mono_clock(mq);
	kvmsg_set_prop(kvmsg, "ttl", "%d", 2); // Set TTL to 2 seconds.
	kvmsg_set_body(kvmsg, (byte*)&msg, sizeof(msg));
	kvmsg_send(kvmsg, zsock_resolve(mq->publisher));
	kvmsg_destroy(&kvmsg);
	if (mq->verbose) {
		LOGI("s_send_pos");
	}
}

void mq_send_fire(LWMESSAGEQUEUE* mq, const float* pos, const float* vel) {
	kvmsg_t* kvmsg = kvmsg_new(0);
	kvmsg_fmt_key(kvmsg, "%s%s/%s/tznt", mq->subtree, zuuid_str(mq->uuid), "fire");
	LWFIREMSG msg;
	memset(&msg, 0, sizeof(LWFIREMSG));
	msg.type = 0x01;
	memcpy(msg.pos, pos, sizeof(msg.pos));
	memcpy(msg.vel, vel, sizeof(msg.vel));
	kvmsg_set_body(kvmsg, (byte*)&msg, sizeof(msg));
	kvmsg_send(kvmsg, zsock_resolve(mq->publisher));
	kvmsg_destroy(&kvmsg);
	if (mq->verbose) {
		LOGI("mq_send_fire");
	}
}

void mq_send_action(LWMESSAGEQUEUE* mq, int action) {
	kvmsg_t* kvmsg = kvmsg_new(0);
	kvmsg_fmt_key(kvmsg, "%s%s/%s/tznt", mq->subtree, zuuid_str(mq->uuid), "action");
	LWACTIONMSG msg;
	msg.type = 0x02;
	msg.action = action;
	kvmsg_set_body(kvmsg, (byte*)&msg, sizeof(msg));
	kvmsg_send(kvmsg, zsock_resolve(mq->publisher));
	kvmsg_destroy(&kvmsg);
	if (mq->verbose) {
		LOGI("mq_send_action");
	}
}

static void s_mq_poll_ready(void* _pLwc, void* _mq, void* sm, void* field) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	LWCONTEXT* pLwc = (LWCONTEXT*)_pLwc;
	zmq_pollitem_t items[] = { { zsock_resolve(mq->subscriber), 0, ZMQ_POLLIN, 0 } };
	double t0 = zclock_mono() / 1e3;
	// Process all queued messages at once
	while (zmq_poll(items, 1, 0) > 0) {
		// Polling subscriber socket to get update
		if (items[0].revents & ZMQ_POLLIN) {
			kvmsg_t* kvmsg = kvmsg_recv(zsock_resolve(mq->subscriber));
			if (!kvmsg) {
				return;
			}

			size_t key_len = strlen(kvmsg_key(kvmsg));
			const static char* TRANSIENT = "/tznt";
			size_t transient_postfix_len = strlen(TRANSIENT);

			if (streq(kvmsg_key(kvmsg), SUBTREE "announce")) {
				show_sys_msg(sm, (char*)kvmsg_body(kvmsg)); // kvmsg body is assumed to be a null terminated string.
				kvmsg_destroy(&kvmsg);
			} else if (key_len > transient_postfix_len && streq(kvmsg_key(kvmsg) + (key_len - transient_postfix_len), TRANSIENT)) {
				int type = *(int*)kvmsg_body(kvmsg);
				switch (type) {
				case 1:
				{
					LWFIREMSG* msg = (LWFIREMSG*)kvmsg_body(kvmsg);
					field_spawn_sphere(field, msg->pos, msg->vel);
					break;
				}
				case 2:
				{
					LWACTIONMSG* msg = (LWACTIONMSG*)kvmsg_body(kvmsg);
					// TODO Update action field only
					break;
				}
				default:
				{
					LOGE("Unknown transient message received: type %d", type);
					break;
				}
				}
				kvmsg_destroy(&kvmsg);
			} else {
				// Discard out-of-sequence kvmsgs, incl. heartbeats
				if (kvmsg_sequence(kvmsg) > mq->sequence) {
					mq->sequence = kvmsg_sequence(kvmsg);
					if (mq->verbose) {
						LOGI("Update: %"PRId64" key:%s, bodylen:%zd\n", mq->sequence, kvmsg_key(kvmsg), kvmsg_size(kvmsg));
					}
					mq_lock_mutex(mq);
					s_kvmsg_store_posmap_noown(&kvmsg, mq->posmap, mq_sync_mono_clock(mq), mq);
					mq_unlock_mutex(mq);
					kvmsg_store(&kvmsg, mq->kvmap);
				} else {
					kvmsg_destroy(&kvmsg);
				}
			}
		}
	}
	double t1 = zclock_mono() / 1e3 - t0;
	//printf("%.3f sec\n", t1);

	if (zclock_time() >= mq->alarm) {
		s_send_pos(pLwc, mq, 0);
		mq->alarm = zclock_time() + 100;
	}
}

void* mq_kvmap(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return mq->kvmap;
}

void* mq_posmap(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return mq->posmap;
}

const LWMQMSG* mq_sync_first(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	kvmsg_t* value = (kvmsg_t*)zhash_first(mq->kvmap);
	if (value) {
		return (const LWMQMSG*)kvmsg_body(value);
	}
	return 0;
}

const char* mq_sync_cursor(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return zhash_cursor(mq->kvmap);
}

const LWMQMSG* mq_sync_next(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	kvmsg_t* value = (kvmsg_t*)zhash_next(mq->kvmap);
	if (value) {
		return (const LWMQMSG*)kvmsg_body(value);
	}
	return 0;
}

LWPOSSYNCMSG* mq_possync_first(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return (LWPOSSYNCMSG*)zhash_first(mq->posmap);
}

const char* mq_possync_cursor(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return zhash_cursor(mq->posmap);
}

LWPOSSYNCMSG* mq_possync_next(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return (LWPOSSYNCMSG*)zhash_next(mq->posmap);
}

const char* mq_uuid_str(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return zuuid_str(mq->uuid);
}

const char* mq_subtree(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return mq->subtree;
}

void mq_poll(void* _pLwc, void* sm, void* _mq, void* field) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	switch (mq->state) {
	case LMQS_INIT:
		break;
	case LMQS_TIME:
		s_mq_poll_time(mq, sm);
		break;
	case LMQS_SNAPSHOT:
		s_mq_poll_snapshot(mq, sm);
		break;
	case LMQS_READY:
		s_mq_poll_ready(_pLwc, mq, sm, field);
		break;
	default:
		break;
	}
}

void deinit_mq(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	zhash_destroy(&mq->kvmap);
	zhash_destroy(&mq->posmap);
	zsock_destroy(&mq->publisher);
	zsock_destroy(&mq->snapshot);
	zsock_destroy(&mq->subscriber);
	zuuid_destroy(&mq->uuid);
	ZMUTEX_DESTROY(mq->mutex);
	mq->state = LMQS_TERM;
	free(mq);
}

void mq_shutdown() {
	zsys_shutdown();
}

void mq_publish_now(void* pLwc, void* _mq, int stop) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	s_send_pos(pLwc, mq, stop);
	//mq->alarm = 0;
}

double mq_mono_clock() {
	return zclock_mono() / 1e3;
}

double mq_sync_mono_clock(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return mq_mono_clock() + mq->delta;
}

int mq_cursor_player(void* _mq, const char* cursor) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	return strcmp(cursor + strlen(mq_subtree(mq)), mq_uuid_str(mq)) == 0;
}

void mq_interrupt() {
	zsys_interrupted = 1;
	zctx_interrupted = 1;
}

void mq_lock_mutex(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	ZMUTEX_LOCK(mq->mutex);
}

void mq_unlock_mutex(void* _mq) {
	LWMESSAGEQUEUE* mq = (LWMESSAGEQUEUE*)_mq;
	ZMUTEX_UNLOCK(mq->mutex);
}
