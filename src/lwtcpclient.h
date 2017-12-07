#pragma once

typedef struct _LWCONTEXT LWCONTEXT;
typedef struct _LWTCP LWTCP;
typedef struct _LWUNIQUEID LWUNIQUEID;

void tcp_on_connect(LWTCP* tcp, const char* path_prefix);
int tcp_send_queue2(LWTCP* tcp, const LWUNIQUEID* id);
int tcp_send_suddendeath(LWTCP* tcp, int battle_id, unsigned int token);
int tcp_send_newuser(LWTCP* tcp);
int tcp_send_querynick(LWTCP* tcp, const LWUNIQUEID* id);
int tcp_send_push_token(LWTCP* tcp, int backoffMs, int domain, const char* push_token);
int tcp_send_get_leaderboard(LWTCP* tcp, int backoffMs, int start_index, int count);
int tcp_send_setnickname(LWTCP* tcp, const LWUNIQUEID* id, const char* nickname);
int tcp_send_cancelqueue(LWTCP* tcp, const LWUNIQUEID* id);
int parse_recv_packets(LWTCP* tcp);
const char* lw_tcp_addr(const LWCONTEXT* pLwc);
const char* lw_tcp_port_str(const LWCONTEXT* pLwc);
int lw_tcp_port(const LWCONTEXT* pLwc);
