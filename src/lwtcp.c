#include "lwtcp.h"
#include "lwlog.h"
#include "puckgamepacket.h"
#include "lwcontext.h"
#include "spherebattlepacket.h"
#include "sysmsg.h"

#if LW_PLATFORM_WIN32
#else
int WSAGetLastError() {
	return -1;
}
#endif

static int make_socket_nonblocking(int sock) {
#if defined(WIN32) || defined(_WIN32) || defined(IMN_PIM)
	unsigned long arg = 1;
	return ioctlsocket(sock, FIONBIO, &arg) == 0;
#elif defined(VXWORKS)
	int arg = 1;
	return ioctl(sock, FIONBIO, (int)&arg) == 0;
#else
	int curFlags = fcntl(sock, F_GETFL, 0);
	return fcntl(sock, F_SETFL, curFlags | O_NONBLOCK) >= 0;
#endif
}

#define NEW_TCP_PACKET(vartype, varname) \
vartype varname; \
varname.size = sizeof(vartype); \
varname.type = LSBPT_##vartype


LWTCP* new_tcp() {
	LWTCP* tcp = (LWTCP*)malloc(sizeof(LWTCP));
	memset(tcp, 0, sizeof(LWTCP));
	tcp->ConnectSocket = INVALID_SOCKET;
	tcp->recvbuflen = LW_TCP_BUFLEN;
	tcp->hints.ai_family = AF_UNSPEC;
	tcp->hints.ai_socktype = SOCK_STREAM;
	tcp->hints.ai_protocol = IPPROTO_TCP;
	
	tcp->iResult = getaddrinfo(LW_TCP_SERVER, LW_TCP_PORT_STR, &tcp->hints, &tcp->result);
	if (tcp->iResult != 0) {
		LOGE("getaddrinfo failed with error: %d", tcp->iResult);
		free(tcp);
		return 0;
	}

	// Attempt to connect to an address until one succeeds
	for (tcp->ptr = tcp->result; tcp->ptr != NULL; tcp->ptr = tcp->ptr->ai_next) {
		// Create a socket for connecting to server
		tcp->ConnectSocket = socket(tcp->ptr->ai_family, tcp->ptr->ai_socktype,
			tcp->ptr->ai_protocol);
		if (tcp->ConnectSocket == INVALID_SOCKET) {
			LOGE("socket failed with error: %ld", WSAGetLastError());
			free(tcp);
			return 0;
		}
		// Connect to server
		tcp->iResult = connect(tcp->ConnectSocket, tcp->ptr->ai_addr, (int)tcp->ptr->ai_addrlen);
		if (tcp->iResult == SOCKET_ERROR) {
			closesocket(tcp->ConnectSocket);
			tcp->ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(tcp->result);
	if (tcp->ConnectSocket == INVALID_SOCKET) {
		LOGE("Unable to connect to server!");
		free(tcp);
		return 0;
	}
	NEW_TCP_PACKET(LWSPHEREBATTLEPACKETQUEUE2, p);
	memcpy(tcp->sendbuf, &p, sizeof(p));
	tcp->iResult = send(tcp->ConnectSocket, tcp->sendbuf, (int)sizeof(p), 0);
	if (tcp->iResult == SOCKET_ERROR) {
		LOGE("send failed with error: %d", WSAGetLastError());
		closesocket(tcp->ConnectSocket);
		free(tcp);
		return 0;
	}
	LOGI("Bytes sent: %ld", tcp->iResult);
#if LW_PLATFORM_WIN32
	/*tcp->iResult = shutdown(tcp->ConnectSocket, SD_SEND);
	if (tcp->iResult == SOCKET_ERROR) {
		LOGI("shutdown failed with error: %d", WSAGetLastError());
		closesocket(tcp->ConnectSocket);
		free(tcp);
		return 0;
	}*/
#endif
	//closesocket(tcp->ConnectSocket);
	//tcp->ConnectSocket = 0;
	make_socket_nonblocking(tcp->ConnectSocket);
	return tcp;
}

void destroy_tcp(LWTCP** tcp) {
	if (*tcp) {
		free(*tcp);
		*tcp = 0;
	}
}

void tcp_send(LWTCP* tcp, const char* data, int size) {
}

#define CHECK_PACKET(packet_type, packet_size, type) \
packet_type == LSBPT_##type && packet_size == sizeof(type)

int parse_recv_packets(LWCONTEXT* pLwc, LWTCP* tcp) {
	// too small for parsing
	if (tcp->recvbufnotparsed < 2) {
		return -1;
	}
	int parsed_bytes = 0;
	char* cursor = tcp->recvbuf;
	while (1) {
		unsigned short packet_size = *(unsigned short*)(cursor + 0);
		// still incomplete packet
		if (packet_size == 0 || packet_size > tcp->recvbufnotparsed - parsed_bytes) {
			return -2;
		}
		unsigned short packet_type = *(unsigned short*)(cursor + 2);
		if (CHECK_PACKET(packet_type, packet_size, LWSPHEREBATTLEPACKETMATCHED2)) {
			LOGI("LWSPHEREBATTLEPACKETMATCHED2 received");
			show_sys_msg(pLwc->def_sys_msg, "LWSPHEREBATTLEPACKETMATCHED2 received");
		} else if (CHECK_PACKET(packet_type, packet_size, LWSPHEREBATTLEPACKETQUEUEOK)) {
			LOGI("LWSPHEREBATTLEPACKETQUEUEOK received");
			show_sys_msg(pLwc->def_sys_msg, "LWSPHEREBATTLEPACKETQUEUEOK received");
		}
		else if (CHECK_PACKET(packet_type, packet_size, LWSPHEREBATTLEPACKETRETRYQUEUE)) {
			LOGI("LWSPHEREBATTLEPACKETRETRYQUEUE received");
			show_sys_msg(pLwc->def_sys_msg, "LWSPHEREBATTLEPACKETRETRYQUEUE received");
		}
		else if (CHECK_PACKET(packet_type, packet_size, LWSPHEREBATTLEPACKETMAYBEMATCHED)) {
			LOGI("LWSPHEREBATTLEPACKETMAYBEMATCHED received");
			show_sys_msg(pLwc->def_sys_msg, "LWSPHEREBATTLEPACKETMAYBEMATCHED received");
		} else {
			LOGE("Unknown TCP packet");
		}
		parsed_bytes += packet_size;
		cursor += packet_size;
	}
	return parsed_bytes;
}

void tcp_update(LWCONTEXT* pLwc, LWTCP* tcp) {
	if (!pLwc || !tcp) {
		return;
	}
	if (LW_TCP_BUFLEN - tcp->recvbufnotparsed <= 0) {
		LOGE("TCP receive buffer overrun!!!");
	}
	int n = recv(tcp->ConnectSocket, tcp->recvbuf + tcp->recvbufnotparsed, LW_TCP_BUFLEN - tcp->recvbufnotparsed, 0);
	if (n > 0) {
		LOGI("TCP received: %d bytes", n);
		tcp->recvbufnotparsed += n;
		int parsed_bytes = parse_recv_packets(pLwc, tcp);
		if (parsed_bytes > 0) {
			for (int i = 0; i < LW_TCP_BUFLEN - parsed_bytes; i++) {
				tcp->recvbuf[i] = tcp->recvbuf[i + parsed_bytes];
			}
			tcp->recvbufnotparsed -= parsed_bytes;
		}
	}
}
