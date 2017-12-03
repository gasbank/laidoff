#pragma once

#include "numcomp.h"

typedef enum _LW_NUMCOMP_FLOAT_TYPE {
    LNFT_PUCK_REFLECT_SIZE,
    LNFT_PUCK_SPEED,
    LNFT_PUCK_MOVE_RAD,
    LNFT_COUNT,
} LW_NUMCOMP_TYPE;

typedef enum _LW_NUMCOMP_VEC3_TYPE {
    LNVT_POS,
    LNVT_COUNT,
} LW_NUMCOMP_VEC3_TYPE;

typedef enum _LW_NUMCOMP_QUATERNION_TYPE {
    LNQT_ROT,
    LNQT_COUNT,
} LW_NUMCOMP_QUATERNION_TYPE;

typedef struct _LWNUMCOMPPUCKGAME {
    LWNUMCOMPFLOATPRESET f[LNFT_COUNT];
    LWNUMCOMPVEC3PRESET v[LNVT_COUNT];
    LWNUMCOMPQUATERNIONPRESET q[LNQT_COUNT];
} LWNUMCOMPPUCKGAME;

void numcomp_puck_game_init(LWNUMCOMPPUCKGAME* numcomp);
