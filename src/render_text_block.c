﻿#include "render_text_block.h"
#include "lwcontext.h"
#include "font.h"
#include "unicode.h"
#include "render_solid.h"
#include "lwlog.h"
#include "laidoff.h"

#define LW_COLOR_WHITE { 1, 1, 1, 1 }
#define LW_COLOR_BLACK { 0, 0, 0, 1 }
#define LW_COLOR_YELLOW { 1, 1, 0, 1 }

//const static float normal_outline_thickness = 3;
//const static float emp_outline_thickness = 3;

float get_proportional_font_size(int height, float font_size) {


	return (float)height / (360.0f) * font_size;
	//return font_size;
}

void render_text_block(const struct _LWCONTEXT *pLwc, const struct _LWTEXTBLOCK* text_block)
{
	int shader_index = LWST_FONT;
	const float screen_aspect_ratio = (float)pLwc->width / pLwc->height;

	glUseProgram(pLwc->shader[shader_index].program);

	glUniform2fv(pLwc->shader[shader_index].vuvoffset_location, 1, default_uv_offset);
	glUniform2fv(pLwc->shader[shader_index].vuvscale_location, 1, default_uv_scale);
	glUniform1f(pLwc->shader[shader_index].alpha_multiplier_location, 1);

	glActiveTexture(GL_TEXTURE0);
	// 0 means GL_TEXTURE0
	glUniform1i(pLwc->shader[shader_index].diffuse_location, 0);
	// default texture param
	glBindTexture(GL_TEXTURE_2D, pLwc->tex_font_atlas[0]);

	glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE,
		(const GLfloat *)pLwc->proj);

	LW_VBO_TYPE lvt = LVT_LEFT_TOP_ANCHORED_SQUARE + (text_block->align - LTBA_LEFT_TOP);
	
	glBindBuffer(GL_ARRAY_BUFFER, pLwc->vertex_buffer[lvt].vertex_buffer);
	bind_all_vertex_attrib_font(pLwc, lvt);
	glUniform2fv(pLwc->shader[shader_index].vuvoffset_location, 1, default_uv_offset);
	glUniform2fv(pLwc->shader[shader_index].vuvscale_location, 1, default_uv_scale);

	mat4x4 model_translate;
	mat4x4 model;
	mat4x4 identity_view;
	mat4x4 view_model;
	mat4x4 proj_view_model;
	mat4x4 model_scale;

	mat4x4_identity(identity_view);

	float size_scaled_xadvance_accum = 0;
	int line = 0;
	int color_emp = 0;

#define MAX_UNICODE_STRLEN (1024)

	unsigned int unicode_str[MAX_UNICODE_STRLEN];
	size_t unicode_strlen = u8_toucs(
		unicode_str,
		ARRAY_SIZE(unicode_str),
		text_block->text + text_block->begin_index,
		text_block->end_index - text_block->begin_index
	);

	if (unicode_strlen >= MAX_UNICODE_STRLEN) {
		LOGE("CRITICAL RUNTIME ERROR! unicode_strlen overflow");
	}
	unicode_str[MAX_UNICODE_STRLEN - 1] = '\0';

	const BMF_CHAR* bc[MAX_UNICODE_STRLEN];
	// Calculate total font block size
	float width_sum = 0;
	float first_width = 0;
	int first_width_set = 0;
	for (size_t i = 0; i < unicode_strlen; i++) {
		bc[i] = font_binary_search_char(pLwc->pFnt, unicode_str[i]);
		if (bc[i]) {
			const float width = bc[i]->xadvance; // : bc[i]->width);
			width_sum += width;

			if (!first_width_set) {
				first_width = width;
				first_width_set = 1;
			}
		}
	}
	bc[unicode_strlen] = 0;

	const int font_base = font_get_base(pLwc->pFnt);
	const int font_line_height = font_get_line_height(pLwc->pFnt);
	float last_x = 0;
	float last_size_scaled_xadvance = 0;

	float align_offset_x = 0;
	float align_offset_y = 0;

	int x_align_mode = 0;
	if (text_block->align == LTBA_LEFT_TOP
		|| text_block->align == LTBA_LEFT_CENTER
		|| text_block->align == LTBA_LEFT_BOTTOM) {
		x_align_mode = -1; // LEFT
	}
	else if (text_block->align == LTBA_CENTER_TOP
		|| text_block->align == LTBA_CENTER_CENTER
		|| text_block->align == LTBA_CENTER_BOTTOM) {

		x_align_mode = 0; // CENTER
		align_offset_x = -width_sum / 2;// +first_width / 2;
		
		//size_scaled_xadvance_accum = (float)first_width / 2;
	}
	else
	{
		x_align_mode = 1; // RIGHT;

		align_offset_x = -width_sum;
	}

	int y_align_mode = 0;
	if (text_block->align == LTBA_LEFT_TOP
		|| text_block->align == LTBA_CENTER_TOP
		|| text_block->align == LTBA_RIGHT_TOP) {
		y_align_mode = 1; // TOP
	}
	else if (text_block->align == LTBA_LEFT_CENTER
		|| text_block->align == LTBA_CENTER_CENTER
		|| text_block->align == LTBA_RIGHT_CENTER) {
		y_align_mode = 0; // CENTER
	}
	else {
		y_align_mode = -1; // BOTTOM
	}

	const float prop_font_size = get_proportional_font_size(pLwc->height, text_block->size);

	align_offset_x = align_offset_x / pLwc->width* 2 * screen_aspect_ratio * prop_font_size;
	align_offset_y = align_offset_y / pLwc->height * 2 * prop_font_size;
	
	for (size_t i = 0; i < unicode_strlen; i++) {
		if (unicode_str[i] == '\n') {
			if (text_block->multiline) {
				size_scaled_xadvance_accum = 0;
				line++;
			} else {
				break;
			}
		}

		const BMF_CHAR* bci = bc[i];
		if (bci) {
			float ui_scale_x = prop_font_size * (float)bci->width / pLwc->width * screen_aspect_ratio;
			float ui_scale_y = prop_font_size * (float)bci->height / pLwc->height;

			mat4x4_identity(model_scale);
			mat4x4_scale_aniso(model_scale, model_scale, ui_scale_x, ui_scale_y, 1.0f);

			float xoffset = ((float)+bci->xoffset + size_scaled_xadvance_accum) / pLwc->width * 2;
			float yoffset = ((float)-bci->yoffset) / pLwc->height * 2;

			if (y_align_mode == 1) {
				yoffset = ((float)-bci->yoffset + font_line_height - font_base) / pLwc->height * 2;
			}
			else if (y_align_mode == 0) {
				yoffset = ((float)font_line_height / 2 - bci->yoffset - bci->height / 2) / pLwc->height * 2;
			}
			else {
				yoffset = ((float)font_base - bci->yoffset - bci->height) / pLwc->height * 2;
			}

			if (x_align_mode == -1) {
				xoffset = ((float)+bci->xoffset + size_scaled_xadvance_accum) / pLwc->width * 2;
			}
			else if (x_align_mode == 0) {
				xoffset = ((float)bci->xoffset + (float)bci->width / 2 + size_scaled_xadvance_accum) / pLwc->width * 2;
			}
			else {
				xoffset = ((float)bci->xoffset + (float)bci->width + size_scaled_xadvance_accum) / pLwc->width * 2;
			}

			const float x = align_offset_x + text_block->text_block_x + prop_font_size * xoffset * screen_aspect_ratio;
			const float y = align_offset_y + text_block->text_block_y + prop_font_size * yoffset + (line * -text_block->text_block_line_height);

			last_x = x;

			mat4x4_translate(model_translate, x, y, 0);

			mat4x4_identity(model);
			mat4x4_mul(model, model_translate, model_scale);

			mat4x4_mul(view_model, identity_view, model);

			mat4x4_identity(proj_view_model);
			mat4x4_mul(proj_view_model, pLwc->proj, view_model);

			glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE,
				(const GLfloat *)proj_view_model);
			glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 0);
			set_tex_filter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

			if (/* DISABLES CODE */ (0) && pLwc->font_texture_texture_mode) {
				glBindTexture(GL_TEXTURE_2D, pLwc->tex_programmed[LPT_SOLID_WHITE_WITH_ALPHA]);
				glUniform2fv(pLwc->shader[shader_index].vuvoffset_location, 1, default_uv_offset);
				glUniform2fv(pLwc->shader[shader_index].vuvscale_location, 1, default_uv_scale);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, pLwc->tex_font_atlas[bci->page]);
				// TODO: 폰트 텍스처 해상도 1024x1024 하드코딩
				set_texture_parameter_values(pLwc, bci->x, bci->y, bci->width, bci->height, 1024, 1024, 1);
			}
			
			if (unicode_str[i] == '<') {
				color_emp = 1;
				continue;
			}

			if (unicode_str[i] == '>') {
				color_emp = 0;
				continue;
			}

			glUniform4fv(pLwc->shader[shader_index].glyph_color_location, 1,
				color_emp ? text_block->color_emp_glyph : text_block->color_normal_glyph);
			glUniform4fv(pLwc->shader[shader_index].outline_color_location, 1,
				color_emp ? text_block->color_emp_outline : text_block->color_normal_outline);

			if (bci->id != ' ') {
				// Vertex buffer is specified outside this loop
				glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
			}

			//const BMF_CHAR* bci_next = bc[i + 1];

			const float outline_xadvance = 0;// (float)(color_emp ? emp_outline_thickness : normal_outline_thickness) / 2;

			if (x_align_mode == -1) {
				last_size_scaled_xadvance = bci->xadvance + outline_xadvance;
			}
			else if (x_align_mode == 0) {
				//last_size_scaled_xadvance = (bci_next ? ((float)-bci->width / 2 + bci->xadvance + bci_next->width / 2) : bci->xadvance) + outline_xadvance;
				last_size_scaled_xadvance = bci->xadvance + outline_xadvance;
				//last_size_scaled_xadvance = (bci_next ? ((float)bci->xadvance / 2 + bci_next->xadvance / 2) : ((float)bci->xadvance / 2)) + outline_xadvance;
			}
			else {
				last_size_scaled_xadvance = bci->xadvance + outline_xadvance;
			}
			size_scaled_xadvance_accum += last_size_scaled_xadvance;

			if (prop_font_size * size_scaled_xadvance_accum / pLwc->width * 2 * screen_aspect_ratio > text_block->text_block_width) {
				size_scaled_xadvance_accum = 0;
				line++;
			}
		}
	}

	glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 0);

	// Render text block debug indicator
	if (pLwc->font_texture_texture_mode) {
		const float aspect_ratio = (float)pLwc->width / pLwc->height;
		const float width_pixels = (float)(2 * aspect_ratio) / pLwc->width;
		const float height_pixels = (float)(2) / pLwc->height;

		//const float w = 2 * width_pixels;
		const float h = 2 * height_pixels;

        // text total width debug indicator
		render_solid_vb_ui(
			pLwc,
			text_block->text_block_x,
			text_block->text_block_y,
			prop_font_size * size_scaled_xadvance_accum * aspect_ratio / pLwc->width * 2,
			h,
			pLwc->tex_programmed[LPT_SOLID_RED],
			lvt,
			1,
			0,
			0,
			0,
			0
		);

		const float w2 = 2 * width_pixels;
		const float h2 = 2 * height_pixels;

        // text origin point debug indicator
		render_solid_vb_ui(
			pLwc,
			text_block->text_block_x,
			text_block->text_block_y,
			w2,
			h2,
			pLwc->tex_programmed[LPT_SOLID_YELLOW],
			LVT_CENTER_CENTER_ANCHORED_SQUARE,
			1,
			0,
			0,
			0,
			0
		);
	}
}

void toggle_font_texture_test_mode(LWCONTEXT *pLwc) {
	pLwc->font_texture_texture_mode = !pLwc->font_texture_texture_mode;
}
