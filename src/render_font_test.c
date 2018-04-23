#include <string.h>
#include "render_font_test.h"
#include "lwcontext.h"
#include "render_text_block.h"
#include "render_solid.h"
#include "laidoff.h"
#include "htmlui.h"
#include "linmath.h"
#include "lwmath.h"
#include <stdio.h>
#include "lwttl.h"
#include "lwlog.h"
#include "lwlnglat.h"

#define WATER_COLOR_R (0 / 255.f)
#define WATER_COLOR_G (94 / 255.f)
#define WATER_COLOR_B (190 / 255.f)

void lwc_render_font_test_fbo_body(const LWCONTEXT* pLwc, const char* html_body) {
    glBindFramebuffer(GL_FRAMEBUFFER, pLwc->font_fbo.fbo);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, pLwc->font_fbo.width, pLwc->font_fbo.height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    htmlui_load_render_draw_body(pLwc->htmlui, html_body);

    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lwc_prerender_font_test_fbo(const LWCONTEXT* pLwc) {
    glBindFramebuffer(GL_FRAMEBUFFER, pLwc->font_fbo.fbo);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, pLwc->font_fbo.width, pLwc->font_fbo.height);
    glClearColor(0, 0, 0, 0); // alpha should be cleared to zero
                              //lw_clear_color();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void lwc_postrender_font_test_fbo(const LWCONTEXT* pLwc) {
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lwc_render_font_test_fbo(const LWCONTEXT* pLwc, const char* html_path) {
    lwc_prerender_font_test_fbo(pLwc);

    //LWTEXTBLOCK test_text_block;
    //test_text_block.text_block_width = 999.0f;// 2.00f * aspect_ratio;
    //test_text_block.text_block_line_height = DEFAULT_TEXT_BLOCK_LINE_HEIGHT_D;
    //test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_B;
    //SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_glyph, 1, 1, 1, 1);
    //SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_outline, 0, 0, 0, 1);
    //SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_glyph, 1, 1, 0, 1);
    //SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_outline, 0, 0, 0, 1);
    //test_text_block.text = LWU("lqpM^_^123-45");
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.multiline = 1;

    //// The first column (left aligned)

    //test_text_block.text_block_x = -0.9f * pLwc->aspect_ratio;
    //test_text_block.text_block_y = 0.25f;
    //test_text_block.align = LTBA_LEFT_TOP;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text_block_x = -0.9f * pLwc->aspect_ratio;
    //test_text_block.text_block_y = 0;
    //test_text_block.align = LTBA_LEFT_CENTER;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text_block_x = -0.9f * pLwc->aspect_ratio;
    //test_text_block.text_block_y = -0.25f;
    //test_text_block.align = LTBA_LEFT_BOTTOM;
    //render_text_block(pLwc, &test_text_block);

    //// The second column (center aligned)

    //test_text_block.text = LWU("lqpM^__^Mpql");
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0;
    //test_text_block.text_block_y = 0.25f;
    //test_text_block.align = LTBA_CENTER_TOP;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text = LWU("가가가가가가가가가가가가가가가가가가가가");
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0;
    //test_text_block.text_block_y = 0.50f;
    //test_text_block.align = LTBA_CENTER_TOP;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text = LWU("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0;
    //test_text_block.text_block_y = 0.75f;
    //test_text_block.align = LTBA_CENTER_TOP;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text = LWU("FBO pre-rendered 한글이 됩니다~~~");
    ////test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_A;
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0;
    //test_text_block.text_block_y = 1.0f;
    //test_text_block.align = LTBA_CENTER_TOP;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text_block_x = 0;
    //test_text_block.text_block_y = 0;
    //test_text_block.align = LTBA_CENTER_CENTER;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text = LWU("이제 진정하십시오...");
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0;
    //test_text_block.text_block_y = -0.25f;
    //test_text_block.align = LTBA_CENTER_BOTTOM;
    //render_text_block(pLwc, &test_text_block);


    //// The third Column (right aligned)

    //test_text_block.text = LWU("lmqpMQ^__^ 123-45");
    ////test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_A;
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0.9f * pLwc->aspect_ratio;
    //test_text_block.text_block_y = 0.25f;
    //test_text_block.align = LTBA_RIGHT_TOP;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text_block_x = 0.9f * pLwc->aspect_ratio;
    //test_text_block.text_block_y = 0;
    //test_text_block.align = LTBA_RIGHT_CENTER;
    //render_text_block(pLwc, &test_text_block);

    //test_text_block.text = LWU("국민 여러분!");
    //test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    //test_text_block.begin_index = 0;
    //test_text_block.end_index = test_text_block.text_bytelen;
    //test_text_block.text_block_x = 0.9f * pLwc->aspect_ratio;
    //test_text_block.text_block_y = -0.25f;
    //test_text_block.align = LTBA_RIGHT_BOTTOM;
    //render_text_block(pLwc, &test_text_block);

    //test_html_ui(pLwc);

    htmlui_load_render_draw(pLwc->htmlui, html_path);

    lwc_postrender_font_test_fbo(pLwc);
}

typedef struct _LWWAVE {
    int valid;
    float y;
    float age;
} LWWAVE;
static LWWAVE wave[5];
float last_wave_spawn = 0;

static void render_port(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float port_y) {
    int shader_index = LWST_DEFAULT_NORMAL_COLOR;
    const LWSHADER* shader = &pLwc->shader[shader_index];
    lazy_glUseProgram(pLwc, shader_index);
    mat4x4 rot;
    mat4x4_identity(rot);

    float sx = 1, sy = 1, sz = 1;
    float x = 0, y = port_y, z = 0;
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

    const LW_VBO_TYPE lvt = LVT_PORT;
    lazy_glBindBuffer(pLwc, lvt);
    bind_all_color_vertex_attrib(pLwc, lvt);
    glUniformMatrix4fv(shader->mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glUniformMatrix4fv(shader->m_location, 1, GL_FALSE, (const GLfloat*)model);
    //glShadeModel(GL_FLAT);
    glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
}

static void render_sea_city(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj) {
    int shader_index = LWST_DEFAULT_NORMAL_COLOR;
    const LWSHADER* shader = &pLwc->shader[shader_index];
    lazy_glUseProgram(pLwc, shader_index);
    mat4x4 rot;
    mat4x4_identity(rot);

    float sx = 1, sy = 1, sz = 1;
    float x = 0, y = 25, z = 0;
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

    const LW_VBO_TYPE lvt = LVT_SEA_CITY;
    lazy_glBindBuffer(pLwc, lvt);
    bind_all_color_vertex_attrib(pLwc, lvt);
    glUniformMatrix4fv(shader->mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glUniformMatrix4fv(shader->m_location, 1, GL_FALSE, (const GLfloat*)model);
    //glShadeModel(GL_FLAT);
    glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
}

static void render_ship(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float x, float y, float z) {
    int shader_index = LWST_DEFAULT_NORMAL_COLOR;
    const LWSHADER* shader = &pLwc->shader[shader_index];
    lazy_glUseProgram(pLwc, shader_index);
    mat4x4 rot;
    mat4x4_identity(rot);
    const int view_scale = lwttl_view_scale(pLwc->ttl);
    const float sx = 0.075f / view_scale;
    const float sy = 0.075f / view_scale;
    const float sz = 0.075f / view_scale;
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

    mat4x4 model_normal_transform;
    mat4x4_identity(model_normal_transform);

    const LW_VBO_TYPE lvt = LVT_SHIP;
    lazy_glBindBuffer(pLwc, lvt);
    bind_all_color_vertex_attrib(pLwc, lvt);
    glUniformMatrix4fv(shader->mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glUniformMatrix4fv(shader->m_location, 1, GL_FALSE, (const GLfloat*)model_normal_transform);
    //glShadeModel(GL_FLAT);
    glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
}

static void render_seaport_icon(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float x, float y, float z, float w, float h) {
    int shader_index = LWST_DEFAULT_NORMAL_COLOR;
    const LWSHADER* shader = &pLwc->shader[shader_index];
    lazy_glUseProgram(pLwc, shader_index);
    mat4x4 rot;
    mat4x4_identity(rot);

    mat4x4 model_normal_transform;
    mat4x4_identity(model_normal_transform);

    float sx = w, sy = h, sz = 1;
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

    const LW_VBO_TYPE lvt = LVT_SEAPORT_ICON;
    lazy_glBindBuffer(pLwc, lvt);
    bind_all_color_vertex_attrib(pLwc, lvt);
    glUniformMatrix4fv(shader->mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glUniformMatrix4fv(shader->m_location, 1, GL_FALSE, (const GLfloat*)model_normal_transform);
    //glShadeModel(GL_FLAT);
    glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
}

static void render_land_cell(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float x, float y, float z, float w, float h) {
    int shader_index = LWST_DEFAULT_NORMAL_COLOR;
    const LWSHADER* shader = &pLwc->shader[shader_index];
    lazy_glUseProgram(pLwc, shader_index);
    mat4x4 rot;
    mat4x4_identity(rot);

    mat4x4 model_normal_transform;
    mat4x4_identity(model_normal_transform);

    float sx = w, sy = h, sz = 1;
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

    const LW_VBO_TYPE lvt = LVT_LAND_CELL;
    lazy_glBindBuffer(pLwc, lvt);
    bind_all_color_vertex_attrib(pLwc, lvt);
    glUniformMatrix4fv(shader->mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glUniformMatrix4fv(shader->m_location, 1, GL_FALSE, (const GLfloat*)model_normal_transform);
    //glShadeModel(GL_FLAT);
    glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);
}

static void render_waves(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float ship_y) {
    if (last_wave_spawn + 1.1f < (float)pLwc->app_time) {
        for (int i = 0; i < ARRAY_SIZE(wave); i++) {
            if (wave[i].valid == 0) {
                last_wave_spawn = (float)pLwc->app_time;
                wave[i].age = 0;
                wave[i].y = -5.5f + ship_y;
                wave[i].valid = 1;
                break;
            }
        }
    }
    float delta_time = (float)deltatime_delta_time(pLwc->render_dt);
    for (int i = 0; i < ARRAY_SIZE(wave); i++) {
        if (wave[i].valid) {
            float alpha = LWMAX(0, sinf(wave[i].age * ((float)M_PI / 2.0f)));
            lw_load_tex(pLwc, LAE_WAVE);
            lw_load_tex(pLwc, LAE_WAVE_ALPHA);
            render_solid_vb_ui_alpha_uv_shader_view_proj(pLwc,
                                                         0,
                                                         wave[i].y,
                                                         3.0f + 0.5f * alpha,
                                                         2.5f,
                                                         LAE_WAVE,
                                                         LAE_WAVE_ALPHA,
                                                         LVT_CENTER_CENTER_ANCHORED_SQUARE,
                                                         alpha,
                                                         1.0f,
                                                         1.0f,
                                                         1.0f,
                                                         1.0f,
                                                         default_uv_offset,
                                                         default_uv_scale,
                                                         LWST_ETC1,
                                                         view,
                                                         proj);
            wave[i].y -= delta_time * 1.5f;
            wave[i].age += delta_time;
            if (wave[i].age > 4.0f) {
                wave[i].valid = 0;
            }
        }
    }
}

static void render_earth(const LWCONTEXT* pLwc, const LWTTLLNGLAT* center, int view_scale) {
    const LW_VBO_TYPE lvt = LVT_EARTH;
    const LW_ATLAS_ENUM lae = LAE_WATER_2048_2048;
    const float scale = 2.5f;
    const float x = pLwc->aspect_ratio - 0.5f;
    const float y = 0.5f;
    const float alpha_multiplier = 0.5f;
    lazy_glBindBuffer(pLwc, lvt);
    lazy_tex_atlas_glBindTexture(pLwc, lae);

    const int shader_index = LWST_DEFAULT;
    lazy_glUseProgram(pLwc, shader_index);
    glUniform2fv(pLwc->shader[shader_index].vuvoffset_location, 1, default_uv_offset);
    glUniform2fv(pLwc->shader[shader_index].vuvscale_location, 1, default_uv_scale);
    glUniform1f(pLwc->shader[shader_index].alpha_multiplier_location, alpha_multiplier);
    glUniform1i(pLwc->shader[shader_index].diffuse_location, 0); // 0 means GL_TEXTURE0
    glUniform1i(pLwc->shader[shader_index].alpha_only_location, 1); // 1 means GL_TEXTURE1
    glUniform3f(pLwc->shader[shader_index].overlay_color_location, 0, 0, 0);
    glUniform1f(pLwc->shader[shader_index].overlay_color_ratio_location, 0);
    glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE, (const GLfloat*)pLwc->proj);

    const int tex_index = pLwc->tex_atlas[lae];
    mat4x4 model_translate;
    mat4x4 model;
    mat4x4 view_model;
    mat4x4 proj_view_model;
    mat4x4 model_scale;
    mat4x4 model_scale_rotate_1;
    mat4x4 model_scale_rotate;
    mat4x4 view_identity;
    mat4x4_identity(view_identity);
    mat4x4_identity(model_scale);
    mat4x4_scale_aniso(model_scale, model_scale, scale, scale, 0.0f);
    mat4x4_rotate_X(model_scale_rotate_1, model_scale, (float)LWDEG2RAD(center->lat));
    mat4x4_rotate_Y(model_scale_rotate, model_scale_rotate_1, (float)LWDEG2RAD(center->lng));
    mat4x4_translate(model_translate, x, y, 0);
    mat4x4_identity(model);
    mat4x4_mul(model, model_translate, model_scale_rotate);
    mat4x4_mul(view_model, view_identity, model);
    mat4x4_identity(proj_view_model);
    mat4x4_mul(proj_view_model, pLwc->proj, view_model);

    lazy_glBindBuffer(pLwc, lvt);
    bind_all_vertex_attrib(pLwc, lvt);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_index);
    //assert(tex_index);
    set_tex_filter(GL_LINEAR, GL_LINEAR);
    //set_tex_filter(GL_NEAREST, GL_NEAREST);
    glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glDrawArrays(GL_TRIANGLES, 0, pLwc->vertex_buffer[lvt].vertex_count);

    //render_solid_box_ui_lvt_flip_y_uv(pLwc,
    //                                  pLwc->aspect_ratio - 0.5f,
    //                                  0.5f,
    //                                  1.0f,
    //                                  1.0f,
    //                                  pLwc->tex_atlas[LAE_WATER_2048_2048],
    //                                  LVT_EARTH,
    //                                  0);

    const float half_extent_in_deg = LNGLAT_SEA_PING_EXTENT_IN_DEGREES / 2 * view_scale;
    const float lng_min = center->lng - half_extent_in_deg;
    const float lng_max = center->lng + half_extent_in_deg;
    const float lat_min = center->lat - half_extent_in_deg;
    const float lat_max = center->lat + half_extent_in_deg;
    const float lng_extent = lng_max - lng_min;
    const float lat_extent = lat_max - lat_min;
    // current view window indicator
    render_solid_vb_ui_flip_y_uv(pLwc,
                                 x,
                                 y,
                                 scale * 2 * sinf((float)LWDEG2RAD(lng_extent) / 2),
                                 scale * 2 * sinf((float)LWDEG2RAD(lat_extent) / 2),
                                 pLwc->tex_atlas[LAE_ZERO_FOR_BLACK],
                                 LVT_CENTER_CENTER_ANCHORED_SQUARE,
                                 0.5f,
                                 0.0f,
                                 1.0f,
                                 0.2f,
                                 1.0f,
                                 0);
}

static void render_sea_objects_nameplate(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, const LWTTLLNGLAT* center) {
    mat4x4 proj_view;
    mat4x4_identity(proj_view);
    mat4x4_mul(proj_view, proj, view);
    const int view_scale = lwttl_view_scale(pLwc->ttl);

    for (int i = 0; i < pLwc->ttl_full_state.count; i++) {
        float x = cell_fx_to_render_coords(pLwc->ttl_full_state.obj[i].x0, center, view_scale);
        float y = cell_fy_to_render_coords(pLwc->ttl_full_state.obj[i].y0, center, view_scale);
        vec4 obj_pos_vec4 = {
            x,
            y,
            0,
            1,
        };
        vec2 ui_point;
        calculate_ui_point_from_world_point(pLwc->aspect_ratio, proj_view, obj_pos_vec4, ui_point);
        LWTEXTBLOCK test_text_block;
        test_text_block.text_block_width = 999.0f;// 2.00f * aspect_ratio;
        test_text_block.text_block_line_height = DEFAULT_TEXT_BLOCK_LINE_HEIGHT_F;
        test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_F;
        SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_glyph, 1, 1, 1, 1);
        SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_outline, 0, 0, 0, 1);
        SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_glyph, 1, 1, 0, 1);
        SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_outline, 0, 0, 0, 1);
        char obj_nameplate[256];
        sprintf(obj_nameplate,
                "[%d][%d](%s) %.2f left",
                pLwc->ttl_full_state.obj[i].id,
                pLwc->ttl_full_state.obj[i].type,
                pLwc->ttl_full_state.obj[i].guid,
                pLwc->ttl_full_state.obj[i].route_left);
        test_text_block.text = obj_nameplate;
        test_text_block.text_bytelen = (int)strlen(test_text_block.text);
        test_text_block.begin_index = 0;
        test_text_block.end_index = test_text_block.text_bytelen;
        test_text_block.multiline = 1;
        test_text_block.text_block_x = ui_point[0];
        test_text_block.text_block_y = ui_point[1];
        test_text_block.align = LTBA_LEFT_TOP;
        render_text_block(pLwc, &test_text_block);
    }
}

static void render_sea_objects(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, const LWTTLLNGLAT* center) {
    const int view_scale = lwttl_view_scale(pLwc->ttl);
    for (int i = 0; i < pLwc->ttl_full_state.count; i++) {
        float x = cell_fx_to_render_coords(pLwc->ttl_full_state.obj[i].x0, center, view_scale);
        float y = cell_fy_to_render_coords(pLwc->ttl_full_state.obj[i].y0, center, view_scale);
        render_ship(pLwc,
                    view,
                    proj,
                    x,
                    y,
                    0);
    }
}

static void render_sea_static_objects(const LWCONTEXT* pLwc,
                                      const mat4x4 view,
                                      const mat4x4 proj,
                                      const LWTTLLNGLAT* center) {
    const int view_scale = lwttl_view_scale(pLwc->ttl);
    const float half_extent_in_deg = LNGLAT_SEA_PING_EXTENT_IN_DEGREES / 2 * view_scale;
    const float lng_min = center->lng - half_extent_in_deg;
    const float lng_max = center->lng + half_extent_in_deg;
    const float lat_min = center->lat - half_extent_in_deg;
    const float lat_max = center->lat + half_extent_in_deg;

    // background sea water
    {
        const float cell_x0 = lng_to_render_coords(lng_min, center, view_scale);
        const float cell_y0 = lat_to_render_coords(lat_max, center, view_scale);
        const float cell_x1 = lng_to_render_coords(lng_max, center, view_scale);
        const float cell_y1 = lat_to_render_coords(lat_min, center, view_scale);
        const float cell_w = cell_x1 - cell_x0;
        const float cell_h = cell_y0 - cell_y1;
        render_solid_vb_ui_uv_shader_rot_view_proj(pLwc,
                                                   0,
                                                   0,
                                                   cell_w,
                                                   cell_h,
                                                   0,
                                                   LVT_CENTER_CENTER_ANCHORED_SQUARE,
                                                   1.0f,
                                                   WATER_COLOR_R,
                                                   WATER_COLOR_G,
                                                   WATER_COLOR_B,
                                                   1.0f,
                                                   default_uv_offset,
                                                   default_uv_scale,
                                                   LWST_DEFAULT,
                                                   0,
                                                   view,
                                                   proj);
    }

    // land
    const int xc0 = lwttl_xc0(pLwc->ttl);
    const int yc0 = lwttl_yc0(pLwc->ttl);
    //lwttl_lock_rendering_mutex(pLwc->ttl);
    for (int i = 0; i < pLwc->ttl_static_state.count; i++) {
        const float lng0_not_clamped = cell_x_to_lng(xc0 + (pLwc->ttl_static_state.obj[i].x0 - xc0) * view_scale);
        const float lat0_not_clamped = cell_y_to_lat(yc0 + (pLwc->ttl_static_state.obj[i].y0 - yc0) * view_scale);
        const float lng1_not_clamped = cell_x_to_lng(xc0 + (pLwc->ttl_static_state.obj[i].x1 - xc0) * view_scale);
        const float lat1_not_clamped = cell_y_to_lat(yc0 + (pLwc->ttl_static_state.obj[i].y1 - yc0) * view_scale);

        const float lng0 = LWCLAMP(lng0_not_clamped, lng_min, lng_max);
        const float lat0 = LWCLAMP(lat0_not_clamped, lat_min, lat_max);
        const float lng1 = LWCLAMP(lng1_not_clamped, lng_min, lng_max);
        const float lat1 = LWCLAMP(lat1_not_clamped, lat_min, lat_max);

        const float cell_x0 = lng_to_render_coords(lng0, center, view_scale);
        const float cell_y0 = lat_to_render_coords(lat0, center, view_scale);
        const float cell_x1 = lng_to_render_coords(lng1, center, view_scale);
        const float cell_y1 = lat_to_render_coords(lat1, center, view_scale);
        const float cell_w = cell_x1 - cell_x0;
        const float cell_h = cell_y0 - cell_y1; // cell_y0 and cell_y1 are in OpenGL rendering coordinates (always cell_y0 > cell_y1)
        // skip degenerated cell
        if (cell_w <= 0 || cell_h <= 0) {
            continue;
        }
        render_land_cell(pLwc,
                         view,
                         proj,
                         cell_x0,
                         cell_y0,
                         0,
                         cell_w,
                         cell_h);
    }
    //lwttl_unlock_rendering_mutex(pLwc->ttl);
    // seaport
    const float cell_scale = 360.0f / LNGLAT_RES_WIDTH;
    for (int i = 0; i < pLwc->ttl_seaport_state.count; i++) {
        render_seaport_icon(pLwc,
                            view,
                            proj,
                            cell_x_to_render_coords(pLwc->ttl_seaport_state.obj[i].x0, center, view_scale),
                            cell_y_to_render_coords(pLwc->ttl_seaport_state.obj[i].y0, center, view_scale),
                            0,
                            cell_scale * sea_render_scale,
                            cell_scale * sea_render_scale);
    }
}

static void render_sea_static_objects_nameplate(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, const LWTTLLNGLAT* center) {
    mat4x4 proj_view;
    mat4x4_identity(proj_view);
    mat4x4_mul(proj_view, proj, view);
    const int view_scale = lwttl_view_scale(pLwc->ttl);
    for (int i = 0; i < pLwc->ttl_seaport_state.count; i++) {
        const float x = cell_x_to_render_coords(pLwc->ttl_seaport_state.obj[i].x0, center, view_scale);
        const float y = cell_y_to_render_coords(pLwc->ttl_seaport_state.obj[i].y0, center, view_scale);
        vec4 obj_pos_vec4 = {
            x,
            y,
            0,
            1,
        };
        vec2 ui_point;
        calculate_ui_point_from_world_point(pLwc->aspect_ratio, proj_view, obj_pos_vec4, ui_point);
        LWTEXTBLOCK test_text_block;
        test_text_block.text_block_width = 999.0f;// 2.00f * aspect_ratio;
        test_text_block.text_block_line_height = DEFAULT_TEXT_BLOCK_LINE_HEIGHT_F;
        test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_F;
        SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_glyph, 1, 1, 1, 1);
        SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_outline, 0, 0, 0, 1);
        SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_glyph, 1, 1, 0, 1);
        SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_outline, 0, 0, 0, 1);
        test_text_block.text = pLwc->ttl_seaport_state.obj[i].name;
        test_text_block.text_bytelen = (int)strlen(test_text_block.text);
        test_text_block.begin_index = 0;
        test_text_block.end_index = test_text_block.text_bytelen;
        test_text_block.multiline = 1;
        test_text_block.text_block_x = ui_point[0];
        test_text_block.text_block_y = ui_point[1];
        test_text_block.align = LTBA_LEFT_BOTTOM;
        render_text_block(pLwc, &test_text_block);
    }
}

static void render_world(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float ship_y, const LWTTLLNGLAT* center) {
    //render_ship(pLwc, view, proj, 0, ship_y, 0);
    render_sea_objects(pLwc, view, proj, center);
    //render_port(pLwc, view, proj, 0);
    //render_port(pLwc, view, proj, 160);
    //render_sea_city(pLwc, view, proj);
    //render_waves(pLwc, view, proj, ship_y);
}

static void render_route_line(const LWCONTEXT* pLwc, const LWTTLWORLDMAP* worldmap, float render_offset_x, float render_offset_y) {
    int shader_index = LWST_LINE;
    float rot = 0.0f;
    lazy_glUseProgram(pLwc, shader_index);

    float ui_scale_x = lnglat_to_xy(pLwc, 1.0f);
    float ui_scale_y = lnglat_to_xy(pLwc, 1.0f);

    mat4x4 model_translate;
    mat4x4 model;
    mat4x4 identity_view; mat4x4_identity(identity_view);
    mat4x4 view_model;
    mat4x4 proj_view_model;
    mat4x4 model_scale;
    mat4x4 model_scale_rotate;

    mat4x4_identity(model_scale);
    mat4x4_scale_aniso(model_scale, model_scale, ui_scale_x, ui_scale_y, 1.0f);
    mat4x4_rotate_Z(model_scale_rotate, model_scale, rot);
    mat4x4_translate(model_translate, worldmap->render_org_x, worldmap->render_org_y, 0);
    mat4x4_identity(model);
    mat4x4_mul(model, model_translate, model_scale_rotate);
    mat4x4_mul(view_model, identity_view, model);
    mat4x4_identity(proj_view_model);
    mat4x4_mul(proj_view_model, pLwc->proj, view_model);

    glBindBuffer(GL_ARRAY_BUFFER, pLwc->sea_route_vbo.vertex_buffer);
    bind_all_line_vertex_attrib(pLwc);
    glUniformMatrix4fv(pLwc->shader[shader_index].mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    int vc = pLwc->sea_route_vbo.vertex_count;
    glDrawArrays(GL_LINE_STRIP, 0, vc);
}

static void render_world_map(const LWCONTEXT* pLwc, const LWTTLWORLDMAP* worldmap) {
    lazy_tex_atlas_glBindTexture(pLwc, LAE_WORLD_MAP);
    //const float uv_offset[2] = { (float)fmod(pLwc->app_time/40.0f, 1.0f), 0 };
    const float uv_scale[2] = {
        1.0f,
        1.0f,
    };
    const float uv_offset[2] = {
        (+worldmap->center.lng / 360.0f),
        (-worldmap->center.lat / 180.0f),
    };

    render_solid_vb_ui_uv_shader_rot(pLwc,
                                     worldmap->render_org_x,
                                     worldmap->render_org_y,
                                     pLwc->aspect_ratio * 2 * worldmap->zoom_scale,
                                     pLwc->aspect_ratio * worldmap->zoom_scale,
                                     pLwc->tex_atlas[LAE_WORLD_MAP],
                                     LVT_CENTER_CENTER_ANCHORED_SQUARE,
                                     1.0f,
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                     uv_offset,
                                     uv_scale,
                                     LWST_DEFAULT,
                                     0);
}

static void render_coords(const LWCONTEXT* pLwc, const LWTTLLNGLAT* lng_lat_center) {
    LWTEXTBLOCK test_text_block;
    test_text_block.text_block_width = 999.0f;// 2.00f * aspect_ratio;
    test_text_block.text_block_line_height = DEFAULT_TEXT_BLOCK_LINE_HEIGHT_F;
    test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_E;
    SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_glyph, 1, 1, 1, 1);
    SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_outline, 0, 0, 0, 1);
    SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_glyph, 1, 1, 0, 1);
    SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_outline, 0, 0, 0, 1);
    char coords[64];
    snprintf(coords, ARRAY_SIZE(coords), "LNG %.3f LAT %.3f", lng_lat_center->lng, lng_lat_center->lat);
    coords[ARRAY_SIZE(coords) - 1] = 0;
    test_text_block.text = coords;
    test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    test_text_block.begin_index = 0;
    test_text_block.end_index = test_text_block.text_bytelen;
    test_text_block.multiline = 1;
    test_text_block.text_block_x = 0.0f;
    test_text_block.text_block_y = 1.0f;
    test_text_block.align = LTBA_CENTER_TOP;
    render_text_block(pLwc, &test_text_block);
}

static void render_region_name(const LWCONTEXT* pLwc) {
    LWTEXTBLOCK test_text_block;
    test_text_block.text_block_width = 999.0f;// 2.00f * aspect_ratio;
    test_text_block.text_block_line_height = DEFAULT_TEXT_BLOCK_LINE_HEIGHT_F;
    test_text_block.size = DEFAULT_TEXT_BLOCK_SIZE_E;
    SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_glyph, 1, 1, 1, 1);
    SET_COLOR_RGBA_FLOAT(test_text_block.color_normal_outline, 0, 0, 0, 1);
    SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_glyph, 1, 1, 0, 1);
    SET_COLOR_RGBA_FLOAT(test_text_block.color_emp_outline, 0, 0, 0, 1);
    test_text_block.text = lwttl_seaarea(pLwc->ttl);
    test_text_block.text_bytelen = (int)strlen(test_text_block.text);
    test_text_block.begin_index = 0;
    test_text_block.end_index = test_text_block.text_bytelen;
    test_text_block.multiline = 1;
    test_text_block.text_block_x = 0.0f;
    test_text_block.text_block_y = 0.9f;
    test_text_block.align = LTBA_CENTER_TOP;
    render_text_block(pLwc, &test_text_block);
}

void lwc_render_font_test(const LWCONTEXT* pLwc) {
    LW_GL_VIEWPORT();
    lw_clear_color();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float ship_y = 0.0f;//+(float)pLwc->app_time;

    float half_height = 10.0f;
    float near_z = 0.1f;
    float far_z = 1000.0f;
    float cam_r = 0;// sinf((float)pLwc->app_time / 4) / 4.0f;
    float c_r = cosf(cam_r);
    float s_r = sinf(cam_r);
    float eye_x = 0;//5.0f;
    float eye_y = 0;//-25.0f;
    float eye_z = 15.0f;
    vec3 eye = { c_r * eye_x - s_r * eye_y, s_r * eye_x + c_r * eye_y, eye_z }; // eye position
    eye[1] += ship_y;
    vec3 center = { 0, ship_y, 0 }; // look position
    vec3 center_to_eye;
    vec3_sub(center_to_eye, eye, center);
    float cam_a = atan2f(center_to_eye[1], center_to_eye[0]);
    //vec3 right = { -sinf(cam_a),cosf(cam_a),0 };
    vec3 right = { cosf(cam_a), -sinf(cam_a), 0 };
    vec3 eye_right;
    vec3_mul_cross(eye_right, center_to_eye, right);
    vec3 up;
    vec3_norm(up, eye_right);
    mat4x4 proj, view;
    const int view_scale = lwttl_view_scale(pLwc->ttl);
    mat4x4_ortho(proj,
                 -half_height * pLwc->aspect_ratio,
                 +half_height * pLwc->aspect_ratio,
                 -half_height,
                 +half_height,
                 near_z,
                 far_z);
    mat4x4_look_at(view, eye, center, up);

    LWTTLLNGLAT lng_lat_center = *lwttl_center(pLwc->ttl);
    // render earth minimap
    render_earth(pLwc, &lng_lat_center, view_scale);
    // render world
    if (lwc_render_font_test_render("landcell")) {
        render_sea_static_objects(pLwc, view, proj, &lng_lat_center);
    }
    if (lwc_render_font_test_render("world")) {
        render_world(pLwc, view, proj, ship_y, &lng_lat_center);
    }
    // UI
    glDisable(GL_DEPTH_TEST);
    if (lwc_render_font_test_render("world")) {
        render_sea_objects_nameplate(pLwc, view, proj, &lng_lat_center);
    }
    if (lwc_render_font_test_render("landcell_nameplate")) {
        render_sea_static_objects_nameplate(pLwc, view, proj, &lng_lat_center);
    }
    lwc_enable_additive_blending();
    const LWTTLWORLDMAP* worldmap = lwttl_worldmap(pLwc->ttl);
    if (lwc_render_font_test_render("worldmap")) {
        render_world_map(pLwc, worldmap);
    }
    lwc_disable_additive_blending();
    if (lwc_render_font_test_render("routeline")) {
        float one_pixel = 2.0f / pLwc->height;
        int thickness = 1;
        for (int i = -thickness; i <= thickness; i++) {
            for (int j = -thickness; j <= thickness; j++) {
                render_route_line(pLwc, worldmap, i * one_pixel, j * one_pixel);
            }
        }
    }
    if (lwc_render_font_test_render("seaport")) {
        lwttl_render_all_seaports(pLwc, pLwc->ttl, worldmap);
    }
    render_coords(pLwc, &lng_lat_center);
    render_region_name(pLwc);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // render FBO (HTML UI)
    render_solid_box_ui_lvt_flip_y_uv(pLwc, 0, 0, 2 * pLwc->aspect_ratio, 2, pLwc->font_fbo.color_tex, LVT_CENTER_CENTER_ANCHORED_SQUARE, 1);
    // render joystick
    if (0) {
        render_dir_pad_with_start_joystick(pLwc, &pLwc->left_dir_pad, 1.0f);
    }
    glEnable(GL_DEPTH_TEST);
}

int lwc_render_font_test_render(const char* name) {
    size_t i;
    for (i = 0; i < MAX_VISIBILITY_ENTRY_COUNT; i++) {
        if (strcmp(visibility[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

void lwc_render_font_test_enable_render(const char* name, int v) {
    size_t i;
    for (i = 0; i < MAX_VISIBILITY_ENTRY_COUNT; i++) {
        if (strcmp(visibility[i], name) == 0) {
            if (v) {
                // already visible...
                return;
            } else {
                // clear
                visibility[i][0] = 0;
                return;
            }
        }
    }
    if (v) {
        for (i = 0; i < MAX_VISIBILITY_ENTRY_COUNT; i++) {
            if (visibility[i][0] == 0) {
                strcpy(visibility[i], name);
                return;
            }
        }
        LOGE("visibility_hash capacity exceeded.");
    }
}
