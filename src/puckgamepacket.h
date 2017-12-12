#pragma once

typedef enum _LW_PUCK_GAME_PACKET {
	LPGP_LWPGETTOKEN = 0,
	LPGP_LWPTOKEN,
	LPGP_LWPQUEUE,
	LPGP_LWPMATCHED,
	LPGP_LWPPLAYERDAMAGED,
	LPGP_LWPTARGETDAMAGED,
	// udp
	LPGP_LWPMOVE = 100,
	LPGP_LWPSTOP,
	LPGP_LWPDASH,
	LPGP_LWPPULLSTART,
	LPGP_LWPPULLSTOP,
	LPGP_LWPSTATE,
    LPGP_LWPJUMP,
    LPGP_LWPFIRE,
    LPGP_LWPSTATE2,
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
    LPGP_LWPSETNICKNAME = 215,
    LPGP_LWPSETNICKNAMERESULT = 216,
    LPGP_LWPSETBATTLEPRESET = 217,
    LPGP_LWPCANCELQUEUE = 218,
    LPGP_LWPCANCELQUEUEOK = 219,
    LPGP_LWPRETRYQUEUELATER = 220,
    // should be less than 256 (packet type field: unsigned char type)
    LPGP_SENTIAL_UNSIGNED_CHAR = 255,
	// internal admin tcp
	LPGP_LWPCREATEBATTLE = 1000,
	LPGP_LWPCREATEBATTLEOK = 1001,
    LPGP_LWPCHECKBATTLEVALID = 1002,
    LPGP_LWPBATTLEVALID = 1003,
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

typedef enum _LWP_STATE_PHASE {
    LSP_READY = 0,
    LSP_STEADY = 1,
    LSP_GO = 2,
    LSP_FINISHED_DRAW = 3,
    LSP_FINISHED_VICTORY_P1 = 4,
    LSP_FINISHED_VICTORY_P2 = 5,
} LWP_STATE_PHASE;

typedef enum _LWP_STATE_WALL_HIT_BIT {
    LSWHB_EAST = 1 << 0,
    LSWHB_WEST = 1 << 1,
    LSWHB_SOUTH = 1 << 2,
    LSWHB_NORTH = 1 << 3,
} LWP_STATE_WALL_HIT_BIT;

#define puck_game_state_phase_finished(v) ((v) >= LSP_FINISHED_DRAW)
#define puck_game_state_phase_started(v) ((v) >= LSP_GO)
#define puck_game_state_phase_battling(v) (puck_game_state_phase_started((v)) && !puck_game_state_phase_finished((v)))

typedef struct _LWPSTATEBITFIELD {
    unsigned int player_current_hp : 4;
    unsigned int player_total_hp : 4;
    unsigned int target_current_hp : 4;
    unsigned int target_total_hp : 4;
    unsigned int puck_owner_player_no : 2;
    unsigned int phase : 3; // LWP_STATE_PHASE
    unsigned int player_pull : 1;
    unsigned int target_pull : 1;
    unsigned int wall_hit_bit : 4; // LWP_STATE_WALL_HIT_BIT (ORing)
} LWPSTATEBITFIELD;

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
    // reflect size
    float puck_reflect_size;
    // bitfield
    LWPSTATEBITFIELD bf;
} LWPSTATE;

typedef struct _LWPSTATE2GAMEOBJECT {
    unsigned int pos; // fixed-point compressed vec3 (x:12-bit, y:12-bit, z:8-bit)
    unsigned int rot; // compressed quaternion
    unsigned short speed; // fixed-point compressed float
    unsigned short move_rad; // fixed-point compressed float
} LWPSTATE2GAMEOBJECT;

// UDP
typedef struct _LWPSTATE2 {
    unsigned char type;
    unsigned char puck_reflect_size; // fixed-point compressed float
    unsigned short update_tick;
    LWPSTATE2GAMEOBJECT go[3]; // [3] --> [0]:puck, [1]:player, [2]:target
    LWPSTATEBITFIELD bf; // bitfield
} LWPSTATE2;

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

typedef struct _LWPCANCELQUEUE {
    unsigned short Size;
    unsigned short Type;
    unsigned int Id[4];
} LWPCANCELQUEUE;

typedef struct _LWPCANCELQUEUEOK {
    unsigned short size;
    unsigned short type;
} LWPCANCELQUEUEOK;

typedef struct _LWPQUEUEOK {
	unsigned short size;
	unsigned short type;
} LWPQUEUEOK;

typedef struct _LWPRETRYQUEUE {
	unsigned short size;
	unsigned short type;
} LWPRETRYQUEUE;

typedef struct _LWPRETRYQUEUELATER {
    unsigned short size;
    unsigned short type;
} LWPRETRYQUEUELATER;

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
	unsigned short Size;
	unsigned short Type;
	unsigned int Id1[4];
	unsigned int Id2[4];
    char Nickname1[LW_NICKNAME_MAX_LEN];
    char Nickname2[LW_NICKNAME_MAX_LEN];
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

typedef struct _LWPCHECKBATTLEVALID {
    unsigned short Size;
    unsigned short Type;
    int Battle_id;
} LWPCHECKBATTLEVALID;

typedef struct _LWPBATTLEVALID {
    unsigned short Size;
    unsigned short Type;
    int Valid;
} LWPBATTLEVALID;

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

typedef struct _LWPSETNICKNAME {
    unsigned short Size;
    unsigned short Type;
    unsigned int Id[4];
    char Nickname[LW_NICKNAME_MAX_LEN];
} LWPSETNICKNAME;

typedef struct _LWPSETNICKNAMERESULT {
    unsigned short Size;
    unsigned short Type;
    unsigned int Id[4];
    char Nickname[LW_NICKNAME_MAX_LEN];
} LWPSETNICKNAMERESULT;

typedef struct _LWPSETBATTLEPRESET {
    unsigned short size;
    unsigned short type;
    // LWPUCKGAME datasheet
    float world_size;
    float dash_interval;
    float dash_duration;
    float dash_shake_time;
    float hp_shake_time;
    float jump_force;
    float jump_interval;
    float jump_shake_time;
    float puck_damage_contact_speed_threshold;
    float sphere_mass;
    float sphere_radius;
    float total_time;
    float fire_max_force;
    float fire_max_vel;
    float fire_interval;
    float fire_duration;
    float fire_shake_time;
    float tower_pos;
    float tower_radius;
    float tower_mesh_radius;
    int tower_total_hp;
    float tower_shake_time;
    float go_start_pos;
    int hp;
    // Camera
    float camera_height;
} LWPSETBATTLEPRESET;

//#pragma pack(pop)
