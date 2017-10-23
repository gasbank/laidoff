#include "puckgame.h"
#include "lwmacro.h"
#include "lwlog.h"
#include "lwcontext.h"
#include "input.h"

static void testgo_move_callback(dBodyID b) {
	LWPUCKGAMEOBJECT* go = (LWPUCKGAMEOBJECT*)dBodyGetData(b);
	LWPUCKGAME* puck_game = go->puck_game;

	// Position
	const dReal* p = dBodyGetPosition(b);
	go->pos[0] = (float)p[0];
	go->pos[1] = (float)p[1];
	go->pos[2] = (float)p[2];
	// Orientation
	const dReal* r = dBodyGetRotation(b); // dMatrix3
	mat4x4 rot;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			rot[i][j] = (float)r[4*i + j];
		}
	}
	rot[3][0] = 0;
	rot[3][1] = 0;
	rot[3][2] = 0;
	rot[3][3] = 1;

	mat4x4_transpose(go->rot, rot);
}

static void create_go(LWPUCKGAME* puck_game, LW_PUCK_GAME_OBJECT lpgo, float mass, float radius, float x, float y) {
	LWPUCKGAMEOBJECT* go = &puck_game->go[lpgo];
	go->puck_game = puck_game;
	go->radius = radius;
	const float testgo_radius = go->radius;
	const float initial_height = testgo_radius * 1;
	go->geom = dCreateSphere(puck_game->space, testgo_radius);
	go->body = dBodyCreate(puck_game->world);
	dMass m;
	dMassSetSphereTotal(&m, mass, testgo_radius);
	dBodySetMass(go->body, &m);
	dBodySetPosition(go->body, x, y, initial_height);
	dBodySetData(go->body, go);
	dBodySetMovedCallback(go->body, testgo_move_callback);
	//dBodySetLinearVel(go->body, -2.0f, 3.0f, 0);
	dGeomSetBody(go->geom, go->body);
	//dBodySetDamping(go->body, 1e-2f, 1e-2f);
	//dBodySetAutoDisableLinearThreshold(go->body, 0.05f);
}

LWPUCKGAME* new_puck_game() {
	// Static game data
	LWPUCKGAME* puck_game = malloc(sizeof(LWPUCKGAME));
	memset(puck_game, 0, sizeof(LWPUCKGAME));
	puck_game->world_size = 4.0f;
	puck_game->world_size_half = puck_game->world_size / 2.0f;
	puck_game->dash_interval = 1.5f;
	puck_game->dash_duration = 0.1f;
	puck_game->dash_speed_ratio = 8.0f;
	// ------

	// Initialize OpenDE
	dInitODE2(0);
	puck_game->world = dWorldCreate();
	puck_game->space = dHashSpaceCreate(0);
	puck_game->boundary[LPGB_GROUND] = dCreatePlane(puck_game->space, 0, 0, 1, 0);
	puck_game->boundary[LPGB_E] = dCreatePlane(puck_game->space, -1, 0, 0, -puck_game->world_size_half);
	puck_game->boundary[LPGB_W] = dCreatePlane(puck_game->space, 1, 0, 0, -puck_game->world_size_half);
	puck_game->boundary[LPGB_S] = dCreatePlane(puck_game->space, 0, -1, 0, -puck_game->world_size_half);
	puck_game->boundary[LPGB_N] = dCreatePlane(puck_game->space, 0, 1, 0, -puck_game->world_size_half);
	dWorldSetGravity(puck_game->world, 0, 0, -9.81f);
	dWorldSetCFM(puck_game->world, 1e-5f);
	
	create_go(puck_game, LPGO_PUCK, 0.1f, 0.125f, 1.0f, 0.0f);
	create_go(puck_game, LPGO_PLAYER, 0.1f, 0.125f, 0.0f, 0.0f);
	create_go(puck_game, LPGO_TARGET, 0.1f, 0.125f, 0.0f, 1.0f);

	puck_game->player_control_joint = dJointCreateLMotor(puck_game->world, puck_game->player_control_joint_group);
	dJointID pcj = puck_game->player_control_joint;
	dJointSetLMotorNumAxes(pcj, 2);
	dJointSetLMotorAxis(pcj, 0, 0, 1, 0, 0); // x-axis actuator
	dJointSetLMotorAxis(pcj, 1, 0, 0, 1, 0); // y-axis actuator
	dJointAttach(pcj, puck_game->go[LPGO_PLAYER].body, 0);
	dJointSetLMotorParam(pcj, dParamFMax1, 10.0f);
	dJointSetLMotorParam(pcj, dParamFMax2, 10.0f);

	puck_game->contact_joint_group = dJointGroupCreate(0);
	puck_game->player_control_joint_group = dJointGroupCreate(0);

	dBodySetKinematic(puck_game->go[LPGO_PUCK].body);

	return puck_game;
}

void delete_puck_game(LWPUCKGAME** puck_game) {
	for (int i = 0; i < LPGB_COUNT; i++) {
		dGeomDestroy((*puck_game)->boundary[i]);
	}
	dJointGroupDestroy((*puck_game)->player_control_joint_group);
	dJointGroupDestroy((*puck_game)->contact_joint_group);
	dSpaceDestroy((*puck_game)->space);
	dWorldDestroy((*puck_game)->world);

	free(*puck_game);
	*puck_game = 0;
}

static void near_puck_player(LWPUCKGAME* puck_game) {
	LWPUCKGAMEOBJECT* puck = &puck_game->go[LPGO_PUCK];
	LWPUCKGAMEOBJECT* player = &puck_game->go[LPGO_PLAYER];
	if (dBodyIsKinematic(puck->body))
	{
		dBodySetDynamic(puck->body);

		const float dx = puck->pos[0] - player->pos[0];
		const float dy = puck->pos[1] - player->pos[1];
		const float fscale = 50.0f;
		dBodyAddForce(puck->body, dx*fscale, dy*fscale, 0);
	}
}

static void near_callback(void *data, dGeomID o1, dGeomID o2)
{
	LWPUCKGAME* puck_game = (LWPUCKGAME*)data;

	if (dGeomIsSpace(o1) || dGeomIsSpace(o2)) {

		// colliding a space with something :
		dSpaceCollide2(o1, o2, data, &near_callback);

		// collide all geoms internal to the space(s)
		if (dGeomIsSpace(o1))
			dSpaceCollide((dSpaceID)o1, data, &near_callback);
		if (dGeomIsSpace(o2))
			dSpaceCollide((dSpaceID)o2, data, &near_callback);

	} else {
		const int max_contacts = 5;
		dContact contact[5];
		// colliding two non-space geoms, so generate contact
		// points between o1 and o2
		int num_contact = dCollide(o1, o2, max_contacts, &contact->geom, sizeof(dContact));
		// add these contact points to the simulation ...
		for (int i = 0; i < num_contact; i++) {

			if (o1 == puck_game->boundary[LPGB_GROUND] || o2 == puck_game->boundary[LPGB_GROUND]) {
				contact[i].surface.mode = dContactSoftCFM | dContactRolling;// | dContactFDir1;
				contact[i].surface.rho = 0.001f;
				contact[i].surface.rho2 = 0.001f;
				contact[i].surface.rhoN = 0.001f;
				//contact[i].fdir1[0] = 1.0f;
				//contact[i].fdir1[1] = 0.0f;
				//contact[i].fdir1[2] = 0.0f;
				contact[i].surface.mu = 100.9f;
			}
			else if (o1 == puck_game->go[LPGO_PUCK].geom && o2 == puck_game->go[LPGO_PLAYER].geom)
			{
				contact[i].surface.mode = dContactSoftCFM | dContactBounce;
				contact[i].surface.mu = 1.9f;

				near_puck_player(puck_game);
			}
			else if (o1 == puck_game->go[LPGO_PLAYER].geom && o2 == puck_game->go[LPGO_PUCK].geom)
			{
				contact[i].surface.mode = dContactSoftCFM | dContactBounce;
				contact[i].surface.mu = 1.9f;

				near_puck_player(puck_game);
			}
			else {
				contact[i].surface.mode = dContactSoftCFM | dContactBounce;
				contact[i].surface.mu = 1.9f;
			}
			

			// bounce is the amount of "bouncyness".
			contact[i].surface.bounce = 0.9f;
			// bounce_vel is the minimum incoming velocity to cause a bounce
			contact[i].surface.bounce_vel = 0.1f;
			// constraint force mixing parameter
			contact[i].surface.soft_cfm = 0.001f;
			dJointID c = dJointCreateContact(puck_game->world, puck_game->contact_joint_group, &contact[i]);
			dBodyID b1 = dGeomGetBody(o1);
			dBodyID b2 = dGeomGetBody(o2);

			dJointAttach(c, b1, b2);
		}

	}
}

void update_puck_game(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, double delta_time) {
	if (!puck_game->world) {
		return;
	}
	puck_game->time += (float)delta_time;
	dSpaceCollide(puck_game->space, puck_game, near_callback);
	//dWorldStep(puck_game->world, 0.005f);
	dWorldQuickStep(puck_game->world, 0.02f);
	dJointGroupEmpty(puck_game->contact_joint_group);
	const dReal* p = dBodyGetPosition(puck_game->go[LPGO_PUCK].body);
	
	//LOGI("pos %.2f %.2f %.2f", p[0], p[1], p[2]);

	dJointID pcj = puck_game->player_control_joint;
	float player_speed = 0.5f;
	//dJointSetLMotorParam(pcj, dParamVel1, player_speed * (pLwc->player_move_right - pLwc->player_move_left));
	//dJointSetLMotorParam(pcj, dParamVel2, player_speed * (pLwc->player_move_up - pLwc->player_move_down));

	//pLwc->player_pos_last_moved_dx
	float dx, dy, dlen;
	if (lw_get_normalized_dir_pad_input(pLwc, &dx, &dy, &dlen)) {
		dJointSetLMotorParam(pcj, dParamVel1, player_speed * dx);
		dJointSetLMotorParam(pcj, dParamVel2, player_speed * dy);
	} else {
		dJointSetLMotorParam(pcj, dParamVel1, 0);
		dJointSetLMotorParam(pcj, dParamVel2, 0);
	}

	// Move direction fixed while dashing
	if (puck_game->dash.remain_time > 0) {
		player_speed *= puck_game->dash_speed_ratio;
		dx = puck_game->dash.dir_x;
		dy = puck_game->dash.dir_y;
		dJointSetLMotorParam(pcj, dParamVel1, player_speed * dx);
		dJointSetLMotorParam(pcj, dParamVel2, player_speed * dy);
		puck_game->dash.remain_time -= (float)delta_time;
	}
}

void puck_game_push(LWPUCKGAME* puck_game) {
	//puck_game->push = 1;

	dBodySetLinearVel(puck_game->go[LPGO_PUCK].body, 1, 1, 0);
}

void puck_game_dash(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
	// Check params
	if (!pLwc || !puck_game) {
		return;
	}
	// Check cooltime
	if (puck_game_dash_cooltime(puck_game) < puck_game->dash_interval) {
		return;
	}
	// Check already effective dash
	if (puck_game->dash.remain_time > 0) {
		return;
	}
	// Check effective move input
	float dx, dy, dlen;
	if (!lw_get_normalized_dir_pad_input(pLwc, &dx, &dy, &dlen)) {
		return;
	}
	// Start dash!
	puck_game->dash.remain_time = puck_game->dash_duration;
	puck_game->dash.dir_x = dx;
	puck_game->dash.dir_y = dy;
	puck_game->dash.last_time = puck_game->time;
}

float puck_game_dash_gauge_ratio(LWPUCKGAME* puck_game) {
	return LWMIN(1.0f, puck_game_dash_cooltime(puck_game) / puck_game->dash_interval);
}

float puck_game_dash_cooltime(LWPUCKGAME* puck_game) {
	return puck_game->time - puck_game->dash.last_time;
}
