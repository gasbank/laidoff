#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "lwcontext.h"
#include "script.h"
#include "file.h"
#include "lwlog.h"
#include "laidoff.h"

int l_ink(lua_State *L)
{
	int x;
	if (lua_gettop(L) >= 0)
	{
		x = (int)lua_tonumber(L, -1);
		lua_pushnumber(L, 2 * x + 1);
	}
	return 1;
}

int l_spawn_blue_cube_wall(lua_State* L)
{
	if (lua_gettop(L) >= 3)
	{
		LWCONTEXT* pLwc = lua_touserdata(L, 1);
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		int r = spawn_field_object(pLwc, x, y, 1, 1, LVT_CUBE_WALL, pLwc->tex_programmed[LPT_SOLID_BLUE], 1, 1, 1, 0);
		lua_pushinteger(L, r);
	}
	return 1;
}

int l_spawn_red_cube_wall(lua_State* L) {
	if (lua_gettop(L) >= 4) {
		LWCONTEXT* pLwc = lua_touserdata(L, 1);
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		int field_event_id = (int)lua_tonumber(L, 4);
		int r = spawn_field_object(pLwc, x, y, 1, 1, LVT_CUBE_WALL, pLwc->tex_programmed[LPT_SOLID_RED], 1, 1, 0.5f, field_event_id);
		lua_pushinteger(L, r);
	}
	return 1;
}

int l_spawn_pump(lua_State* L) {
	if (lua_gettop(L) >= 4) {
		LWCONTEXT* pLwc = lua_touserdata(L, 1);
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		int field_event_id = (int)lua_tonumber(L, 4);
		int r = spawn_field_object(pLwc, x, y, 1, 1, LVT_PUMP, pLwc->tex_programmed[LPT_SOLID_RED], 1, 1, 0.5f, field_event_id);
		lua_pushinteger(L, r);
	}
	return 1;
}

int l_spawn_oil_truck(lua_State* L) {
	if (lua_gettop(L) >= 4) {
		LWCONTEXT* pLwc = lua_touserdata(L, 1);
		float x = (float)lua_tonumber(L, 2);
		float y = (float)lua_tonumber(L, 3);
		int field_event_id = (int)lua_tonumber(L, 4);
		int r = spawn_field_object(pLwc, x, y, 1, 1, LVT_OIL_TRUCK, pLwc->tex_atlas[LAE_3D_OIL_TRUCK_TEX_KTX], 1, 1, 1.0f, field_event_id);
		lua_pushinteger(L, r);
	}
	return 1;
}

void init_lua(LWCONTEXT* pLwc)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L, l_ink);
	lua_setglobal(L, "ink");
	lua_pushcfunction(L, l_spawn_blue_cube_wall);
	lua_setglobal(L, "spawn_blue_cube_wall");
	lua_pushcfunction(L, l_spawn_red_cube_wall);
	lua_setglobal(L, "spawn_red_cube_wall");
	lua_pushcfunction(L, l_spawn_pump);
	lua_setglobal(L, "spawn_pump");
	lua_pushcfunction(L, l_spawn_oil_truck);
	lua_setglobal(L, "spawn_oil_truck");
	lua_pushlightuserdata(L, pLwc);
	lua_setglobal(L, "pLwc");

	{
		int result = luaL_dostring(L, "return ink(1000)");
		if (result)
		{
			fprintf(stderr, "Failed to run lua: %s\n", lua_tostring(L, -1));
		}
		else
		{
			printf("Lua result: %lld\n", lua_tointeger(L, -1));
		}
		lua_pop(L, 1);
	}

	pLwc->L = L;
	//lua_close(L);

}

void spawn_all_field_object(LWCONTEXT* pLwc) {
	char* script = create_string_from_file(ASSETS_BASE_PATH "l" PATH_SEPARATOR "spawn.lua");
	if (script) {
		int result = luaL_dostring(pLwc->L, script);
		release_string(script);
		if (result) {
			fprintf(stderr, "Failed to run lua: %s\n", lua_tostring(pLwc->L, -1));
		} else {
			printf("Lua result: %lld\n", lua_tointeger(pLwc->L, -1));
		}
		lua_pop(pLwc->L, 1);
	} else {
		LOGE("spawn_all_field_object: loading script failed");
	}
}

void despawn_all_field_object(LWCONTEXT* pLwc) {
	for (int i = 0; i < ARRAY_SIZE(pLwc->field_object); i++) {
		pLwc->field_object[i].valid = 0;
	}
	
	for (int i = 0; i < ARRAY_SIZE(pLwc->box_collider); i++) {
		pLwc->box_collider[i].valid = 0;
	}
}
