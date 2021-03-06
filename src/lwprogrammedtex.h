#pragma once

typedef enum {
	LPT_GRID,
	LPT_SOLID_RED,
	LPT_SOLID_GREEN,
	LPT_SOLID_BLUE,
	LPT_SOLID_EXP_COLOR,
	LPT_SOLID_GRAY,
	LPT_SOLID_BLACK,
	LPT_SOLID_YELLOW,
	LPT_SOLID_TRANSPARENT, // testing
	LPT_SOLID_WHITE_WITH_ALPHA, // testing
	LPT_DIR_PAD,
	LPT_BOTH_END_GRADIENT_HORIZONTAL,

	LPT_COUNT,
} LW_PROGRAMMED_TEX;
#define MAX_TEX_PROGRAMMED LPT_COUNT
