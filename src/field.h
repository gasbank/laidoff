#pragma once
#include <ode/ode.h>
#include "lwgl.h"
#include "lwanim.h"
#include "nav.h"
#include "lwvbotype.h"

#define MAX_BOX_GEOM (100)
#define MAX_CENTER_RAY_RESULT (10)
#define MAX_CONTACT_RAY_RESULT (10)
#define MAX_FIELD_CONTACT (10)

typedef struct _LWFIELDCUBEOBJECT {
	float x, y, z;
	float dimx, dimy, dimz;
	float axis_angle[4];
} LWFIELDCUBEOBJECT;

typedef struct _LWFIELD {
	dWorldID world;
	dSpaceID space;
	dGeomID ground;
	dGeomID box_geom[MAX_BOX_GEOM];
	int box_geom_count;
	dGeomID player_geom;
	dGeomID player_center_ray;
	dGeomID player_contact_ray;
	dReal player_radius;
	dReal player_length;
	dContact center_ray_result[MAX_CENTER_RAY_RESULT];
	int center_ray_result_count;
	dContact contact_ray_result[MAX_CONTACT_RAY_RESULT];
	int contact_ray_result_count;
	dVector3 player_pos;
	dVector3 player_pos_delta;
	dVector3 ground_normal;
	LWFIELDCUBEOBJECT* field_cube_object;
	int field_cube_object_count;
	char* d;

	void* nav;
	LWPATHQUERY path_query;
	float path_query_time;
	vec3 path_query_test_player_pos;
	float path_query_test_player_rot;

	LW_VBO_TYPE field_vbo;
	GLuint field_tex_id;
	int field_tex_mip;
	float skin_scale;
	int follow_cam;
} LWFIELD;

void move_player(LWCONTEXT *pLwc);
void resolve_player_collision(LWCONTEXT *pLwc);
LWFIELD* load_field(const char* filename);
void unload_field(LWFIELD* field);
void update_field(LWCONTEXT* pLwc, LWFIELD* field);
void set_field_player_delta(LWFIELD* field, float x, float y, float z);
void set_field_player_position(LWFIELD* field, float x, float y, float z);
void get_field_player_position(const LWFIELD* field, float* x, float* y, float* z);
void field_attack(LWCONTEXT* pLwc);
