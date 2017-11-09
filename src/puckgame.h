#pragma once
#include "lwatlasenum.h"
#include "lwvbotype.h"
#include "armature.h"
#include "lwanim.h"
#include "puckgamepacket.h"
#include <ode/ode.h>

typedef struct _LWPUCKGAME LWPUCKGAME;

typedef enum _LW_PUCK_GAME_BOUNDARY {
	LPGB_GROUND, LPGB_E, LPGB_W, LPGB_S, LPGB_N,
	LPGB_COUNT
} LW_PUCK_GAME_BOUNDARY;

typedef enum _LW_PUCK_GAME_OBJECT {
	LPGO_PUCK,
	LPGO_PLAYER,
	LPGO_TARGET,
	LPGO_COUNT,
} LW_PUCK_GAME_OBJECT;

typedef struct _LWPUCKGAMEOBJECT {
	dGeomID geom;
	dBodyID body;
	float pos[3]; // read-only
	float move_rad; // angle on x-y plane
	float radius;
	mat4x4 rot;
	LWPUCKGAME* puck_game;
	int wall_hit_count;
	float speed;
	int red_overlay;
} LWPUCKGAMEOBJECT;

typedef struct _LWPUCKGAMEDASH {
	float last_time;
	float remain_time;
	float dir_x;
	float dir_y;
	float shake_remain_time;
} LWPUCKGAMEDASH;

typedef struct _LWPUCKGAMEPLAYER {
	int total_hp;
	int current_hp;
	dBodyID last_contact_puck_body;
	int puck_contacted;
	float hp_shake_remain_time;
} LWPUCKGAMEPLAYER;

typedef struct _LWREMOTEPLAYERCONTROL {
	int dir_pad_dragging;
	float dx;
	float dy;
	int pull_puck;
} LWREMOTEPLAYERCONTROL;

typedef struct _LWPUCKGAME {
	// Static game data
	float world_size;
	float world_size_half;
	float dash_interval;
	float dash_duration;
	float dash_speed_ratio;
	float dash_shake_time;
	float hp_shake_time;
	float puck_damage_contact_speed_threshold;
    float sphere_mass;
    float sphere_radius;
	// ----
	dWorldID world;
	dSpaceID space;
	dGeomID boundary[LPGB_COUNT];
	LWPUCKGAMEOBJECT go[LPGO_COUNT];
	dJointGroupID contact_joint_group;
	dJointGroupID  player_control_joint_group;
	dJointGroupID  target_control_joint_group;
	dJointGroupID  puck_pull_control_joint_group;
	dJointID player_control_joint; // player 1
	dJointID target_control_joint; // player 2
	dJointID puck_pull_control_joint;
	int push;
	float time;
	LWPUCKGAMEDASH dash;
	LWPUCKGAMEPLAYER player;
	LWPUCKGAMEPLAYER target;
	float last_remote_dx;
	float last_remote_dy;
	int pull_puck;
	int remote;
	void(*on_player_damaged)(LWPUCKGAME*);
	void(*on_target_damaged)(LWPUCKGAME*);
	void* server;
	int battle_id;
	unsigned int token;
	unsigned int player_no;
	unsigned int c1_token;
	unsigned int c2_token;
	LWREMOTEPLAYERCONTROL remote_control[2];
	LWPUCKGAMEDASH remote_dash[2];
	int init_ready;
	int update_tick;
	char nickname[LW_NICKNAME_MAX_LEN];
} LWPUCKGAME;

LWPUCKGAME* new_puck_game();
void delete_puck_game(LWPUCKGAME** puck_game);
void puck_game_push(LWPUCKGAME* puck_game);
float puck_game_dash_gauge_ratio(LWPUCKGAME* puck_game);
float puck_game_dash_cooltime(LWPUCKGAME* puck_game);
int puck_game_dashing(LWPUCKGAMEDASH* dash);
void puck_game_near_callback(void *data, dGeomID o1, dGeomID o2);
void puck_game_commit_dash(LWPUCKGAME* puck_game, LWPUCKGAMEDASH* dash, float dx, float dy);
void puck_game_commit_dash_to_puck(LWPUCKGAME* puck_game, LWPUCKGAMEDASH* dash, int player_no);
void puck_game_player_decrease_hp_test(LWPUCKGAME* puck_game);
void puck_game_target_decrease_hp_test(LWPUCKGAME* puck_game);
float puck_game_remain_time(int update_tick);
void puck_game_go_decrease_hp_test(LWPUCKGAME* puck_game, LWPUCKGAMEPLAYER* go, LWPUCKGAMEDASH* dash);