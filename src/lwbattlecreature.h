﻿#pragma once

#include "unicode.h"
#include "lwattrib.h"
#include "lwskill.h"

#define MAX_SKILL_PER_CREATURE (8)

typedef struct _LWCREATURESTAT {
	LWATTRIBVALUE attrib;
	int str; // Strength
	int agl; // Agility
	int mag; // Magic
	int stm; // Stamina
	int spd; // Speed
	int lck; // Luck
} LWCREATURESTAT;

#define CREATURESTAT_GRADE_1_NON_WEAK { {LW_NON_WEAK}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_1_AIR_WEAK { {LW_AIR_WEAK | LW_WAT_IMMU}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_1_WAT_WEAK { {LW_WAT_WEAK | LW_AIR_RESI}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_1_FIR_WEAK { {LW_FIR_WEAK}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_1_EAR_WEAK { {LW_EAR_WEAK}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_1_GOD_WEAK { {LW_GOD_WEAK}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_1_EVL_WEAK { {LW_EVL_WEAK}, 10, 10, 10, 10, 10, 10 }
#define CREATURESTAT_GRADE_2_NON_WEAK { {LW_NON_WEAK}, 12, 11, 12, 11, 12, 11 }
#define CREATURESTAT_GRADE_2_AIR_WEAK { {LW_AIR_WEAK}, 12, 11, 12, 11, 12, 11 }
#define CREATURESTAT_GRADE_2_WAT_WEAK { {LW_WAT_WEAK}, 12, 11, 12, 11, 12, 11 }
#define CREATURESTAT_GRADE_2_FIR_WEAK { {LW_FIR_WEAK}, 12, 11, 12, 11, 12, 11 }
#define CREATURESTAT_GRADE_2_EAR_WEAK { {LW_EAR_WEAK}, 12, 11, 12, 11, 12, 11 }
#define CREATURESTAT_GRADE_2_GOD_WEAK { {LW_GOD_WEAK}, 12, 11, 12, 11, 12, 11 }
#define CREATURESTAT_GRADE_2_EVL_WEAK { {LW_EVL_WEAK}, 12, 11, 12, 11, 12, 11 }


typedef struct _LWBATTLECREATURE {
	int valid;
	
	// datatime variables
	char name[64];
	int lv;
	int hp;
	int max_hp;
	int mp;
	int max_mp;
	LWCREATURESTAT stat;

	const LWSKILL* skill[MAX_SKILL_PER_CREATURE];

	// runtime variables
	int selected;
	float selected_r;
	float selected_g;
	float selected_b;
	float selected_a;
	int turn_token;
	int turn_consumed;

} LWBATTLECREATURE;

#define BATTLECREATURE_ICECREAM { 1, LWU("아이스크림"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_HANNIBAL { 1, LWU("한니발"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_AIR_WEAK }
#define BATTLECREATURE_KEYBOARD { 1, LWU("키보드"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_WAT_WEAK }
#define BATTLECREATURE_FISH { 1, LWU("물고기"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_FIR_WEAK }
#define BATTLECREATURE_ANIMAL { 1, LWU("동물"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_EAR_WEAK }
#define BATTLECREATURE_GRASS { 1, LWU("풀"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_GOD_WEAK }
#define BATTLECREATURE_BAMBOO { 1, LWU("대나무"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_EVL_WEAK }
#define BATTLECREATURE_CUP { 1, LWU("컵"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_NON_WEAK }
#define BATTLECREATURE_STRANGE_ANIMAL { 1, LWU("뒤집힌 동물"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_AIR_WEAK }
#define BATTLECREATURE_HUMAN { 1, LWU("사람"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_WAT_WEAK }
#define BATTLECREATURE_WEANLING_ANIMAL { 1, LWU("어린동물"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_FIR_WEAK }
#define BATTLECREATURE_SOLAR_SYSTEM { 1, LWU("태양계"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_EAR_WEAK }
#define BATTLECREATURE_SPEAKER { 1, LWU("스피커"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_GOD_WEAK }
#define BATTLECREATURE_WATCH { 1, LWU("손목시계"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_2_EVL_WEAK }
#define BATTLECREATURE_STICK { 1, LWU("메모리스틱"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SAFE { 1, LWU("금고"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_NAIL_CLIPPERS { 1, LWU("손톱깎이"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_CAT { 1, LWU("고양이"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_BOOKSHELF { 1, LWU("책장"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_EARPHONES { 1, LWU("이어폰"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SELLER { 1, LWU("판매자"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_BUYER { 1, LWU("구매자"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_DRIVER { 1, LWU("운전자"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_COTTON_SWAB { 1, LWU("면봉"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_FEATURE_PHONE { 1, LWU("피처폰"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_LIPGLOSS { 1, LWU("립글로스"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_BIKER { 1, LWU("폭죽족"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_DOOR { 1, LWU("방문"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_CHAIR { 1, LWU("의자"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_TRAFFIC_LIGHTS { 1, LWU("신호등"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_BOX { 1, LWU("상자"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_MADAM { 1, LWU("아줌마"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_GATE { 1, LWU("차단기"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_TREE { 1, LWU("가로수"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SHOWER { 1, LWU("샤워기"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SEMICONDUCTOR { 1, LWU("반도체"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SPOON_AND_CHOPSTICKS { 1, LWU("수저"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_LADY { 1, LWU("여인"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_MAYO { 1, LWU("마요"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_PUNK { 1, LWU("펑크족"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_ELEVATOR { 1, LWU("승강기"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_RESEARCHER { 1, LWU("연구원"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_WRITER { 1, LWU("대문호"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SQUID { 1, LWU("오징어"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_SHOE { 1, LWU("신발"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
#define BATTLECREATURE_TOFU { 1, LWU("두부"), 1, 1, 1, 10, 10, CREATURESTAT_GRADE_1_NON_WEAK }
