#include "lwcontext.h"
#include "render_physics.h"
#include "render_solid.h"
#include "laidoff.h"
#include "lwlog.h"
#include "puckgame.h"
#include "render_field.h"

static void render_go(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, const LWPUCKGAMEOBJECT* go) {
	int shader_index = LWST_DEFAULT;
	const int tex_index = pLwc->tex_atlas[LAE_PUCK_KTX];
	mat4x4 rot;
	mat4x4_identity(rot);
	float sx = go->radius, sy = go->radius, sz = go->radius;
	float x = go->pos[0], y = go->pos[1], z = go->pos[2];

	mat4x4 model;
	mat4x4_identity(model);
	mat4x4_mul(model, model, rot);
	mat4x4_scale_aniso(model, model, sx, sy, sz);
	//mat4x4_scale_aniso(model, model, 5, 5, 5);

	mat4x4 model_translate;
	mat4x4_translate(model_translate, x, y, z);

	mat4x4_mul(model, go->rot, model);
	mat4x4_mul(model, model_translate, model);

	mat4x4 view_model;
	mat4x4_mul(view_model, view, model);

	mat4x4 proj_view_model;
	mat4x4_identity(proj_view_model);
	mat4x4_mul(proj_view_model, proj, view_model);

	glUniform3f(pLwc->shader[shader_index].overlay_color_location, 1, 1, 1);
	glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 0);

	const LW_VBO_TYPE lvt = LVT_PUCK;
	glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 0);
	glBindBuffer(GL_ARRAY_BUFFER, pLwc->vertex_buffer[lvt].vertex_buffer);
	bind_all_vertex_attrib(pLwc, lvt);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_index);
	set_tex_filter(GL_LINEAR, GL_LINEAR);
	//set_tex_filter(GL_NEAREST, GL_NEAREST);
	glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
	glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
}

void lwc_render_physics(const struct _LWCONTEXT* pLwc) {
	glViewport(0, 0, pLwc->width, pLwc->height);
	lw_clear_color();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int shader_index = LWST_DEFAULT;

	const float screen_aspect_ratio = (float)pLwc->width / pLwc->height;

	mat4x4 proj;
	mat4x4_perspective(proj, (float)(LWDEG2RAD(49.134) / screen_aspect_ratio), screen_aspect_ratio, 1.0f, 500.0f);

	mat4x4 view;
	vec3 eye = { 0.0f, -2.0f, 8.5f };

	vec3 center = { 0, 0, 0 };

	vec3 up = { 0, 1, 0 };
	mat4x4_look_at(view, eye, center, up);

	glUseProgram(pLwc->shader[shader_index].program);
	glUniform2fv(pLwc->shader[shader_index].vuvoffset_location, 1, default_uv_offset);
	glUniform2fv(pLwc->shader[shader_index].vuvscale_location, 1, default_uv_scale);
	glUniform2fv(pLwc->shader[shader_index].vs9offset_location, 1, default_uv_offset);
	glUniform1f(pLwc->shader[shader_index].alpha_multiplier_location, 1.0f);
	glUniform1i(pLwc->shader[shader_index].diffuse_location, 0); // 0 means GL_TEXTURE0
	glUniform1i(pLwc->shader[shader_index].alpha_only_location, 1); // 1 means GL_TEXTURE1
	glUniform3f(pLwc->shader[shader_index].overlay_color_location, 1, 1, 1);
	glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 0);
	

	const LWPUCKGAME* puck_game = pLwc->puck_game;

	{
		const int tex_index = 0;
		mat4x4 rot;
		mat4x4_identity(rot);

		float sx = 2.0f, sy = 2.0f, sz = 2.0f;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		mat4x4 model;
		mat4x4_identity(model);
		mat4x4_mul(model, model, rot);
		mat4x4_scale_aniso(model, model, sx, sy, sz);
		mat4x4 model_translate;
		mat4x4_translate(model_translate, x, y, z);
		mat4x4_mul(model, model_translate, model);

		mat4x4 view_model;
		mat4x4_mul(view_model, view, model);

		mat4x4 proj_view_model;
		mat4x4_identity(proj_view_model);
		mat4x4_mul(proj_view_model, proj, view_model);

		glUniform3f(pLwc->shader[shader_index].overlay_color_location, 0.5f, 0.5f, 0.5f);
		glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 1.0f);

		//glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);

		const LW_VBO_TYPE lvt = LVT_CENTER_CENTER_ANCHORED_SQUARE;
		
		glBindBuffer(GL_ARRAY_BUFFER, pLwc->vertex_buffer[lvt].vertex_buffer);
		bind_all_vertex_attrib(pLwc, lvt);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_index);
		set_tex_filter(GL_LINEAR, GL_LINEAR);
		//set_tex_filter(GL_NEAREST, GL_NEAREST);
		glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
		glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
	}

	render_go(pLwc, view, proj, &puck_game->go[LPGO_PUCK]);
	render_go(pLwc, view, proj, &puck_game->go[LPGO_PLAYER]);

	render_dir_pad(pLwc);
}
