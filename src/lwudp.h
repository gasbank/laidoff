#pragma once
#include "platform_detection.h"
#if LW_PLATFORM_WIN32
#include <WinSock2.h>
#else
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <stdlib.h>
#include <endian.h>
#include <czmq_prelude.h>
#endif
#define LW_UDP_SERVER "52.231.31.72"
#define LW_UDP_BUFLEN 512
#define LW_UDP_PORT 19856

typedef struct _LWUDP {
#if LW_PLATFORM_WIN32
	WSADATA wsa;
#endif
	struct sockaddr_in si_other;
	int s, slen;
	char buf[LW_UDP_BUFLEN];
	char message[LW_UDP_BUFLEN];
	fd_set readfds;
	struct timeval tv;
	int recv_len;
	int ready;

} LWUDP;

typedef struct _LWCONTEXT LWCONTEXT;

LWUDP* new_udp();
void destroy_udp(LWUDP** udp);
void udp_send(LWUDP* udp, const char* data, int size);
void udp_update(LWCONTEXT* pLwc, LWUDP* udp);
