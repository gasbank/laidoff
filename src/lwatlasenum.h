#pragma once

#include "lwmacro.h"

typedef enum _LW_ATLAS_ENUM
{
    LAE_TWIRL_PNG,
    LAE_C2_PNG,
    LAE_TWIRL_XXX_PNG,
    LAE_BG_ROAD_PNG,
    
    LAE_BG_KITCHEN,
    LAE_BG_MART_IN,
    LAE_BG_MART_OUT,
    LAE_BG_ROAD,
    LAE_BG_ROOM,
    LAE_BG_ROOM_CEILING,
    
    LAE_P_DOHEE,
    LAE_P_DOHEE_ALPHA,
    LAE_P_MOTHER,
    LAE_P_MOTHER_ALPHA,
    
    LAE_U_DIALOG_BALLOON,
    
    LAE_BG_KITCHEN_KTX,
    LAE_BG_MART_IN_KTX,
    LAE_BG_MART_OUT_KTX,
    LAE_BG_ROAD_KTX,
    LAE_BG_ROOM_KTX,
    LAE_BG_ROOM_CEILING_KTX,
    
    LAE_FX_TRAIL,
    LAE_FX_TRAIL_ALPHA,
    
    LAE_U_GLOW,
    LAE_U_GLOW_ALPHA,

    LAE_U_ENEMY_SCOPE_KTX,
    LAE_U_ENEMY_SCOPE_ALPHA_KTX,
	LAE_U_ENEMY_TURN_KTX,
	LAE_U_ENEMY_TURN_ALPHA_KTX,
	LAE_U_PLAYER_TURN_KTX,
	LAE_U_PLAYER_TURN_ALPHA_KTX,
	LAE_U_FIST_ICON_KTX,
	LAE_U_FIST_ICON_ALPHA_KTX,

	LAE_C_BIKER_KTX,
	LAE_C_BIKER_ALPHA_KTX,
	LAE_C_MADAM_KTX,
	LAE_C_MADAM_ALPHA_KTX,
	LAE_C_SPEAKER_KTX,
	LAE_C_SPEAKER_ALPHA_KTX,
	LAE_C_TOFU_KTX,
	LAE_C_TOFU_ALPHA_KTX,
	LAE_C_TREE_KTX,
	LAE_C_TREE_ALPHA_KTX,

	LAE_3D_PLAYER_TEX_KTX,
	LAE_3D_FLOOR_TEX_KTX,
	LAE_3D_FLOOR2_TEX_KTX,
	LAE_3D_APT_TEX_MIP_KTX,
	LAE_3D_OIL_TRUCK_TEX_KTX,
	LAE_3D_ROOM_TEX_KTX,
	LAE_3D_BATTLEGROUND_FLOOR_BAKE_TEX_KTX,
	LAE_3D_BATTLEGROUND_WALL_BAKE_TEX_KTX,
	LAE_GUNTOWER_KTX,

	LAE_BEAM_KTX,
    
    LAE_COUNT,
} LW_ATLAS_ENUM;

static const char *tex_atlas_filename[] = {
	ASSETS_BASE_PATH "tex" PATH_SEPARATOR "Twirl.png",
	ASSETS_BASE_PATH "tex" PATH_SEPARATOR "atlas01.png",
	ASSETS_BASE_PATH "tex" PATH_SEPARATOR "Twirl.png",
	ASSETS_BASE_PATH "tex" PATH_SEPARATOR "bg-road.png",

	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "bg-kitchen.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "bg-mart-in.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "bg-mart-out.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "bg-road.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "bg-room.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "bg-room-ceiling.pkm",

	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "p-dohee.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "p-dohee_alpha.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "p-mother.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "p-mother_alpha.pkm",

	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "u-dialog-balloon.pkm",

	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "bg-kitchen.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "bg-mart-in.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "bg-mart-out.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "bg-road.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "bg-room.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "bg-room-ceiling.ktx",

	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "fx-trail.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "fx-trail_alpha.pkm",

	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "u-glow.pkm",
	ASSETS_BASE_PATH "pkm" PATH_SEPARATOR "u-glow_alpha.pkm",

	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-enemy-scope-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-enemy-scope-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-enemy-turn-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-enemy-turn-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-player-turn-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-player-turn-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-fist-icon-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "u-fist-icon-a_alpha.ktx",

	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-biker-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-biker-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-madam-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-madam-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-speaker-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-speaker-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-tofu-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-tofu-a_alpha.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-tree-a.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "c-tree-a_alpha.ktx",

	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-player-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-floor-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-floor2-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-apt-tex-mip.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-oil-truck-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-room-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-battleground-floor-bake-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "3d-battleground-wall-bake-tex.ktx",
	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "guntower.ktx",

	ASSETS_BASE_PATH "ktx" PATH_SEPARATOR "fx-beam.ktx",
};

#define MAX_TEX_ATLAS LAE_COUNT

LwStaticAssert(ARRAY_SIZE(tex_atlas_filename) == LAE_COUNT, "LAE_COUNT error");
