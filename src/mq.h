#pragma once

void* init_mq();
void deinit_mq(void* _mq);
void mq_poll(void* pLwc, void* sm, void* _mq);
void init_czmq();
void mq_shutdown();

typedef struct _LWMQMSG {
	float x;
	float y;
	float z;
	float a;
	double t;
} LWMQMSG;

const LWMQMSG* mq_sync_first(void* _mq);
const char* mq_sync_cursor(void* _mq);
const LWMQMSG* mq_sync_next(void* _mq);

const void* mq_possync_first(void* _mq);
const char* mq_possync_cursor(void* _mq);
const void* mq_possync_next(void* _mq);

const char* mq_uuid_str(void* _mq);
const char* mq_subtree(void* _mq);

void mq_publish_now(void* _mq);