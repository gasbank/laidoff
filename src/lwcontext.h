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
#include "lwtimepoint.h"
#include "lwprogrammedtex.h"
#include "lwbattlestate.h"
#include "lwgamescene.h"
#include "lwanim.h"
#include "lwtouchproc.h"
#include "lwbox2dcollider.h"
#include "lwfieldobject.h"
#include "lwfbo.h"
#include "lwtrail.h"

#define MAX_SHADER (3)
#define MAX_ANIM_COUNT (10)
#define ANIM_FPS (60)
#define MAX_TOUCHPROC_COUNT (10)

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
#define MAX_DAMAGE_TEXT (16)
#define MAX_BATTLE_CREATURE (4)
#define MAX_ENEMY_SLOT (5)
#define VERTEX_BUFFER_COUNT LVT_COUNT


typedef struct _LWCONTEXT {
	struct GLFWwindow* window;
	int width;
	int height;

	LWSHADER shader[MAX_SHADER];
	LWVBO vertex_buffer[VERTEX_BUFFER_COUNT];
	//GLuint index_buffer;
	GLuint vao[VERTEX_BUFFER_COUNT];

	GLuint tex_atlas[MAX_TEX_ATLAS];
	int tex_atlas_width[MAX_TEX_ATLAS];
	int tex_atlas_height[MAX_TEX_ATLAS];
	GLuint tex_font_atlas[MAX_TEX_FONT_ATLAS];
	unsigned long tex_atlas_hash[MAX_TEX_ATLAS];
	GLuint tex_programmed[MAX_TEX_PROGRAMMED];

	int rotate;
	mat4x4 mvp;

	LWTIMEPOINT last_time;
	double delta_time;

	mat4x4 proj;
	
	LW_GAME_SCENE game_scene;
	LW_GAME_SCENE next_game_scene;

	LWANIM anim[MAX_ANIM_COUNT];
	LWTOUCHPROC touch_proc[MAX_TOUCHPROC_COUNT];

	int update_count;
	int render_count;

	double app_time;

	int completed;
	int current_heart;
	int max_heart;

	int tex_atlas_index;
	LWSPRITE* sprite_data;

	int selected_command_slot;

	int selected_enemy_slot;
	LWENEMY enemy[MAX_ENEMY_SLOT];
	LW_BATTLE_STATE battle_state;
	LWANIM1D command_in_progress_anim;

	void* pFnt;

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

	int player_move_left;
	int player_move_right;
	int player_move_up;
	int player_move_down;

	float player_pos_x;
	float player_pos_y;

	LWBOX2DCOLLIDER box_collider[MAX_BOX_COLLIDER];
	LWFIELDOBJECT field_object[MAX_FIELD_OBJECT];

	float dir_pad_x;
	float dir_pad_y;
	int dir_pad_dragging;

	LWTRAIL trail[MAX_TRAIL];
	float battle_fov_deg;
	float battle_fov_deg_0;
	float battle_fov_mag_deg_0;
	float battle_cam_center_x;

	LWDAMAGETEXT damage_text[MAX_DAMAGE_TEXT];

	LWBATTLECREATURE player_creature[MAX_BATTLE_CREATURE];

	int player_turn_creature_index;

	float battle_wall_tex_v;

	mat4x4 battle_proj;
	mat4x4 battle_view;

	int font_texture_texture_mode;

	LWFBO font_fbo;

	LWBUTTONCOMMAND admin_button_command[6 * 5];

	float last_mouse_press_x;
	float last_mouse_press_y;
	float last_mouse_move_x;
	float last_mouse_move_y;
} LWCONTEXT;
