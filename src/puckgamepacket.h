#pragma once

typedef enum _LW_PUCK_GAME_PACKET {
	LPGP_LWPGETTOKEN = 0,
	LPGP_LWPTOKEN,
	LPGP_LWPQUEUE,
	LPGP_LWPMATCHED,
	LPGP_LWPPLAYERDAMAGED,
	LPGP_LWPTARGETDAMAGED,
	
	LPGP_LWPMOVE = 100,
	LPGP_LWPSTOP,
	LPGP_LWPDASH,
	LPGP_LWPPULLSTART,
	LPGP_LWPPULLSTOP,
	LPGP_LWPSTATE,
    LPGP_LWPJUMP,
    LPGP_LWPFIRE,

	// tcp
	LPGP_LWPQUEUE2 = 200,
	LPGP_LWPMAYBEMATCHED = 201,
	LPGP_LWPMATCHED2 = 202,
	LPGP_LWPQUEUEOK = 203,
	LPGP_LWPRETRYQUEUE = 204,
	LPGP_LWPSUDDENDEATH = 205,
	LPGP_LWPNEWUSER = 206,
	LPGP_LWPNEWUSERDATA = 207,
	LPGP_LWPQUERYNICK = 208,
	LPGP_LWPNICK = 209,
    LPGP_LWPPUSHTOKEN = 210,
    LPGP_LWPSYSMSG = 211,
    LPGP_LWPGETLEADERBOARD = 212,
    LPGP_LWPLEADERBOARD = 213,
    LPGP_LWPBATTLERESULT = 214,

	// internal admin tcp
	LPGP_LWPCREATEBATTLE = 1000,
	LPGP_LWPCREATEBATTLEOK = 1001,
} LW_PUCK_GAME_PACKET;

// Client -> Server
typedef struct _LWPGETTOKEN {
	int type;
} LWPGETTOKEN;

// Server --> Client
typedef struct _LWPTOKEN {
	int type;
	int token;
} LWPTOKEN;

// Client --> Server
typedef struct _LWPQUEUE {
	int type;
	int token;
} LWPQUEUE;

// Server --> Client
typedef struct _LWPMATCHED {
	int type;
	int master;
} LWPMATCHED;

typedef struct _LWPPLAYERDAMAGED {
	int type;
} LWPPLAYERDAMAGED;

typedef struct _LWPTARGETDAMAGED {
	int type;
} LWPTARGETDAMAGED;

typedef struct _LWPUDPHEADER {
	int type;
	int battle_id;
	int token;
} LWPUDPHEADER;

// UDP
typedef struct _LWPMOVE {
	int type;
	int battle_id;
	int token;
	float dx;
	float dy;
    float dlen;
} LWPMOVE;

// UDP
typedef struct _LWPSTOP {
	int type;
	int battle_id;
	int token;
} LWPSTOP;

// UDP
typedef struct _LWPDASH {
	int type;
	int battle_id;
	int token;
} LWPDASH;

// UDP
typedef struct _LWPFIRE {
    int type;
    int battle_id;
    int token;
    float dx;
    float dy;
    float dlen;
} LWPFIRE;

// UDP
typedef struct _LWPJUMP {
    int type;
    int battle_id;
    int token;
} LWPJUMP;

// UDP
typedef struct _LWPPULLSTART {
	int type;
	int battle_id;
	int token;
} LWPPULLSTART;

// UDP
typedef struct _LWPPULLSTOP {
	int type;
	int battle_id;
	int token;
} LWPPULLSTOP;

// UDP
typedef struct _LWPSTATE {
	int type;
	int update_tick;
	// player
	float player[3];
	float player_rot[4][4];
	float player_speed;
	float player_move_rad;
	// puck
	float puck[3];
	float puck_rot[4][4];
	float puck_speed;
	float puck_move_rad;
	// target
	float target[3];
	float target_rot[4][4];
	float target_speed;
	float target_move_rad;
	// HP
	int player_current_hp;
	int player_total_hp;
	int target_current_hp;
	int target_total_hp;
	// state
	int finished;
} LWPSTATE;

// should be 4-byte aligned...
// (Cgo compatibility issue)
//#pragma pack(push, 1)
typedef struct _LWPNEWUSER {
	unsigned short size;
	unsigned short type;
} LWPNEWUSER;

typedef struct _LWPQUERYNICK {
	unsigned short Size;
	unsigned short Type;
	unsigned int Id[4];
} LWPQUERYNICK;

#define LW_NICKNAME_MAX_LEN (32)
#define LW_LEADERBOARD_ITEMS_IN_PAGE (20)

typedef struct _LWPNICK {
	unsigned short size;
	unsigned short type;
	char nickname[LW_NICKNAME_MAX_LEN];
} LWPNICK;

typedef struct _LWPNEWUSERDATA {
	unsigned short size;
	unsigned short type;
	unsigned int id[4];
	char nickname[32];
} LWPNEWUSERDATA;

typedef struct _LWPQUEUE2 {
	unsigned short Size;
	unsigned short Type;
	unsigned int Id[4];
} LWPQUEUE2;

typedef struct _LWPQUEUEOK {
	unsigned short size;
	unsigned short type;
} LWPQUEUEOK;

typedef struct _LWPRETRYQUEUE {
	unsigned short size;
	unsigned short type;
} LWPRETRYQUEUE;

typedef struct _LWPMAYBEMATCHED {
	unsigned short size;
	unsigned short type;
} LWPMAYBEMATCHED;

typedef struct _LWPMATCHED2 {
	unsigned short size;
	unsigned short type;
	unsigned short port;
	unsigned short padding_unused;
	unsigned char ipaddr[4];
	int battle_id;
	unsigned int token;
	int player_no;
	char target_nickname[LW_NICKNAME_MAX_LEN];
} LWPMATCHED2;

typedef struct _LWPBASE {
	unsigned short size;
	unsigned short type;
} LWPBASE;

typedef struct _LWPCREATEBATTLE {
	unsigned short size;
	unsigned short type;
	unsigned int id1[4];
	unsigned int id2[4];
    char nickname1[LW_NICKNAME_MAX_LEN];
    char nickname2[LW_NICKNAME_MAX_LEN];
} LWPCREATEBATTLE;

typedef struct _LWPCREATEBATTLEOK {
	unsigned short Size;
	unsigned short Type;
	int Battle_id;
	unsigned int C1_token;
	unsigned int C2_token;
	unsigned char IpAddr[4];
	unsigned short Port;
	unsigned short Padding_unused;
} LWPCREATEBATTLEOK;

typedef struct _LWPSUDDENDEATH {
	unsigned short Size;
	unsigned short Type;
	int Battle_id;
	unsigned int Token;
} LWPSUDDENDEATH;

typedef struct _LWPGETLEADERBOARD {
    unsigned short Size;
    unsigned short Type;
    int Start_index;
    int Count;
} LWPGETLEADERBOARD;

typedef struct _LWPLEADERBOARD {
    unsigned short Size;
    unsigned short Type;
    int Count;
    int First_item_rank;
    int First_item_tie_count;
    char Nickname[LW_LEADERBOARD_ITEMS_IN_PAGE][LW_NICKNAME_MAX_LEN];
    int Score[LW_LEADERBOARD_ITEMS_IN_PAGE];
} LWPLEADERBOARD;

enum {
    LW_PUSH_TOKEN_LENGTH = 256,
    LW_SYS_MSG_LENGTH = 256,
};

typedef struct _LWPPUSHTOKEN {
    unsigned short Size;
    unsigned short Type;
    unsigned int Id[4];
    int Domain;
    char Push_token[LW_PUSH_TOKEN_LENGTH];
} LWPPUSHTOKEN;

typedef struct _LWPSYSMSG {
    unsigned short size;
    unsigned short type;
    char message[LW_SYS_MSG_LENGTH];
} LWPSYSMSG;

typedef struct _LWPBATTLERESULT {
    unsigned short Size;
    unsigned short Type;
    int Winner; // 0:draw, 1:Id1 wins, 2:Id2 wins
    unsigned int Id1[4];
    unsigned int Id2[4];
    char Nickname1[LW_NICKNAME_MAX_LEN];
    char Nickname2[LW_NICKNAME_MAX_LEN];
} LWPBATTLERESULT;

//#pragma pack(pop)
