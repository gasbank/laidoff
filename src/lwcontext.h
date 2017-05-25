#pragma once

#include "lwgl.h"
#include "linmath.h"
#include "lwdamagetext.h"
#include "lwbattlecreature.h"
#include "lwenemy.h"
#include "lwshader.h"
#include "lwvbo.h"
#include "lwvbotype.h"
#include "lwmacro.h"
#include "lwatlasenum.h"
#include "lwbuttoncommand.h"
#include "lwprogrammedtex.h"
#include "lwbattlestate.h"
#include "lwgamescene.h"
#include "lwanim.h"
#include "lwtouchproc.h"
#include "lwbox2dcollider.h"
#include "lwfieldobject.h"
#include "lwfbo.h"
#include "lwtrail.h"
#include "armature.h"
#include "playersm.h"
#include "lwdeltatime.h"

typedef enum _LW_SHADER_TYPE {
	LWST_DEFAULT,
	LWST_FONT,
	LWST_ETC1,
	LWST_SKIN,
	LWST_FAN,

	LWST_COUNT,
} LW_SHADER_TYPE;

#define MAX_SHADER (LWST_COUNT)
#define MAX_ANIM_COUNT (10)
#define ANIM_FPS (60)
#define MAX_TOUCHPROC_COUNT (10)

const static vec4 EXP_COLOR = { 90 / 255.0f, 173 / 255.0f, 255 / 255.0f, 1 };

static const char* tex_font_atlas_filename[] = {
	//ASSETS_BASE_PATH "fnt" PATH_SEPARATOR "arita-semi-bold_0.tga",
	//ASSETS_BASE_PATH "fnt" PATH_SEPARATOR "arita-semi-bold_1.tga",
	ASSETS_BASE_PATH "fnt" PATH_SEPARATOR "test6_0.tga",
	ASSETS_BASE_PATH "fnt" PATH_SEPARATOR "test6_1.tga",
};

#define MAX_TEX_FONT_ATLAS (ARRAY_SIZE(tex_font_atlas_filename))


#define MAX_FIELD_OBJECT (32)
#define MAX_BOX_COLLIDER MAX_FIELD_OBJECT
#define MAX_TRAIL (16)
#define SET_COLOR_RGBA_FLOAT(v, r, g, b, a) \
	v[0] = r; \
	v[1] = g; \
	v[2] = b; \
	v[3] = a;
#define SET_COLOR_RGBA_FLOAT_ARRAY(v, rgba) \
	v[0] = rgba[0]; \
	v[1] = rgba[1]; \
	v[2] = rgba[2]; \
	v[3] = rgba[3];
#define MAX_DAMAGE_TEXT (16)
#define MAX_PLAYER_SLOT (4)
#define MAX_ENEMY_SLOT (5)
#define VERTEX_BUFFER_COUNT LVT_COUNT
#define SKIN_VERTEX_BUFFER_COUNT LSVT_COUNT
#define FAN_VERTEX_BUFFER_COUNT LFVT_COUNT
#define MAX_DELTA_TIME_HISTORY (60)

typedef struct _LWCONTEXT {
	struct GLFWwindow* window;
	int width;
	int height;

	LWSHADER shader[MAX_SHADER];
	LWVBO vertex_buffer[VERTEX_BUFFER_COUNT];
	LWVBO skin_vertex_buffer[SKIN_VERTEX_BUFFER_COUNT];
	LWVBO fan_vertex_buffer[FAN_VERTEX_BUFFER_COUNT];
	//GLuint index_buffer;
	GLuint vao[VERTEX_BUFFER_COUNT];
	GLuint skin_vao[SKIN_VERTEX_BUFFER_COUNT];
	GLuint fan_vao[SKIN_VERTEX_BUFFER_COUNT];

	GLuint tex_atlas[MAX_TEX_ATLAS];
	int tex_atlas_width[MAX_TEX_ATLAS];
	int tex_atlas_height[MAX_TEX_ATLAS];
	GLuint tex_font_atlas[MAX_TEX_FONT_ATLAS];
	unsigned long tex_atlas_hash[MAX_TEX_ATLAS];
	GLuint tex_programmed[MAX_TEX_PROGRAMMED];

	int rotate;
	mat4x4 mvp;

	LWDELTATIME* update_dt;
	LWDELTATIME* render_dt;

	mat4x4 proj;
	
	LW_GAME_SCENE game_scene;
	LW_GAME_SCENE next_game_scene;

	LWANIM anim[MAX_ANIM_COUNT];
	LWTOUCHPROC touch_proc[MAX_TOUCHPROC_COUNT];

	int update_count;
	int render_count;

	int kp; // last keypad number (debug)

	double app_time;
	double scene_time;

	int tex_atlas_index;
	LWSPRITE* sprite_data;

	
	void* pFnt;

	// Dialog Mode Components
	char* dialog;
	int dialog_bytelen;
	int render_char;
	double last_render_char_incr_time;
	int dialog_line;
	int dialog_start_index;
	int dialog_next_index;
	int dialog_move_next;
	int dialog_bg_tex_index;
	int dialog_portrait_tex_index;

	// Field Mode Components
	int player_move_left;
	int player_move_right;
	int player_move_up;
	int player_move_down;

	float player_pos_x;
	float player_pos_y;
	float player_pos_last_moved_dx;
	float player_pos_last_moved_dy;
	//int player_moving; // Player is moving
	//int player_attacking; // Player is attacking with its fist
	//int player_aiming; // Player is aiming with its pistol
	const LWANIMACTION* player_action;
	//float player_aim_theta;
	LWPLAYERSTATEDATA player_state_data;

	LWBOX2DCOLLIDER box_collider[MAX_BOX_COLLIDER];
	LWFIELDOBJECT field_object[MAX_FIELD_OBJECT];

	float dir_pad_x;
	float dir_pad_y;
	int dir_pad_dragging;
	int atk_pad_dragging;

	int field_event_id;

	// Battle Mode Components

	LWTRAIL trail[MAX_TRAIL];
	float battle_fov_deg;
	float battle_fov_deg_0;
	float battle_fov_mag_deg_0;
	float battle_cam_center_x;

	int selected_command_slot;
	int selected_enemy_slot;
	LW_BATTLE_STATE battle_state;
	LWANIM1D command_banner_anim;
	LWDAMAGETEXT damage_text[MAX_DAMAGE_TEXT];
	LWBATTLECREATURE player[MAX_PLAYER_SLOT];
	LWENEMY enemy[MAX_ENEMY_SLOT];
	int player_turn_creature_index;
	int enemy_turn_creature_index;
	float enemy_turn_command_wait_time;
	float battle_wall_tex_v;
	mat4x4 battle_proj;
	mat4x4 battle_view;
	LWANIM1D center_image_anim;
	LW_ATLAS_ENUM center_image;

	// Font

	int font_texture_texture_mode;

	LWFBO font_fbo;

	// Input

	float last_mouse_press_x;
	float last_mouse_press_y;
	float last_mouse_move_x;
	float last_mouse_move_y;

	// Admin

	LWBUTTONCOMMAND admin_button_command[6 * 5];

	// Script

	void* L; // lua VM

	// Skin

	LWARMATURE armature[LWAR_COUNT];
	LWANIMACTION action[LWAC_COUNT];
	float player_skin_time;
	float test_player_skin_time;
	int player_action_loop;

	// Field

	struct _LWFIELD* field;

	int fps_mode;
	int hide_field;

	void* def_sys_msg;
	void* mq;

	int server_index;
	int ray_test;
} LWCONTEXT;

#ifdef __cplusplus
extern "C" {;
#endif
double lwcontext_delta_time(const LWCONTEXT* pLwc);
#ifdef __cplusplus
};
#endif
