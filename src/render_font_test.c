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

int enable_render_world = 1;
int enable_render_world_map = 0;

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

void lwc_render_font_test_fbo(const LWCONTEXT* pLwc, const char* html_path) {
    glBindFramebuffer(GL_FRAMEBUFFER, pLwc->font_fbo.fbo);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, pLwc->font_fbo.width, pLwc->font_fbo.height);
    glClearColor(0, 0, 0, 0); // alpha should be cleared to zero
    //lw_clear_color();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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

    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    float sx = 1, sy = 1, sz = 1;
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

    const LW_VBO_TYPE lvt = LVT_SHIP;
    lazy_glBindBuffer(pLwc, lvt);
    bind_all_color_vertex_attrib(pLwc, lvt);
    glUniformMatrix4fv(shader->mvp_location, 1, GL_FALSE, (const GLfloat*)proj_view_model);
    glUniformMatrix4fv(shader->m_location, 1, GL_FALSE, (const GLfloat*)model);
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

static void render_sea_objects_nameplate(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj) {
    mat4x4 proj_view;
    mat4x4_identity(proj_view);
    mat4x4_mul(proj_view, proj, view);
    for (int i = 0; i < pLwc->ttl_full_state.count; i++) {
        vec4 obj_pos_vec4 = {
            pLwc->ttl_full_state.obj[i].x0,
            pLwc->ttl_full_state.obj[i].y0,
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
        char obj_nameplate[64];
        sprintf(obj_nameplate, "SHIP%d", pLwc->ttl_full_state.obj[i].id);
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

static void render_sea_objects(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj) {
    for (int i = 0; i < pLwc->ttl_full_state.count; i++) {
        render_ship(pLwc,
                    view,
                    proj,
                    pLwc->ttl_full_state.obj[i].x0,
                    pLwc->ttl_full_state.obj[i].y0,
                    0);
    }
}

static void render_world(const LWCONTEXT* pLwc, const mat4x4 view, const mat4x4 proj, float ship_y) {
    //render_ship(pLwc, view, proj, 0, ship_y, 0);
    render_sea_objects(pLwc, view, proj);
    render_port(pLwc, view, proj, 0);
    render_port(pLwc, view, proj, 160);
    render_sea_city(pLwc, view, proj);
    //render_waves(pLwc, view, proj, ship_y);
}

static void render_route_line(const LWCONTEXT* pLwc, float x, float y) {
    int shader_index = LWST_LINE;
    float rot = 0.0f;
    lazy_glUseProgram(pLwc, shader_index);
    
    float ui_scale_x = 1.0f / 180.0f * pLwc->aspect_ratio;
    float ui_scale_y = 1.0f / 180.0f * pLwc->aspect_ratio;

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
    mat4x4_translate(model_translate, x, y, 0);
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

static void render_world_map(const LWCONTEXT* pLwc, float x, float y) {
    lazy_tex_atlas_glBindTexture(pLwc, LAE_WORLD_MAP);
    render_solid_box_ui_lvt_flip_y_uv(pLwc, x, y, pLwc->aspect_ratio * 2, pLwc->aspect_ratio, pLwc->tex_atlas[LAE_WORLD_MAP], LVT_CENTER_CENTER_ANCHORED_SQUARE, 0);
}

void lwc_render_font_test(const LWCONTEXT* pLwc) {
    LW_GL_VIEWPORT();
    glClearColor(0 / 255.f, 94 / 255.f, 190 / 255.f, 1);//lw_clear_color();
    //glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float ship_y = 0.0f;//+(float)pLwc->app_time;

    float half_height = 35.0f;
    float near_z = 0.1f;
    float far_z = 1000.0f;
    float cam_r = sinf((float)pLwc->app_time / 4) / 4.0f;
    float c_r = cosf(cam_r);
    float s_r = sinf(cam_r);
    float eye_x = 50.0f;
    float eye_y = -25.0f;
    vec3 eye = { c_r * eye_x - s_r * eye_y, s_r * eye_x + c_r * eye_y, 50.0f }; // eye position
    eye[1] += ship_y;
    vec3 center = { 0, ship_y, 0 }; // look position
    vec3 center_to_eye;
    vec3_sub(center_to_eye, eye, center);
    float cam_a = atan2f(center_to_eye[1], center_to_eye[0]);
    vec3 right = { -sinf(cam_a),cosf(cam_a),0 };
    vec3 eye_right;
    vec3_mul_cross(eye_right, center_to_eye, right);
    vec3 up;
    vec3_norm(up, eye_right);
    mat4x4 proj, view;
    mat4x4_ortho(proj,
                 -half_height * pLwc->aspect_ratio,
                 +half_height * pLwc->aspect_ratio,
                 -half_height,
                 +half_height,
                 near_z,
                 far_z);
    mat4x4_look_at(view, eye, center, up);

    // render world
    if (enable_render_world) {
        render_world(pLwc, view, proj, ship_y);
    }
    // UI
    glDisable(GL_DEPTH_TEST);
    if (enable_render_world) {
        render_sea_objects_nameplate(pLwc, view, proj);
    }
    lwc_enable_additive_blending();
    float world_map_x = 0;
    float world_map_y = -(2.0f - pLwc->aspect_ratio)/2;
    if (enable_render_world_map) {
        render_world_map(pLwc, world_map_x, world_map_y);
    }
    lwc_disable_additive_blending();
    if (enable_render_world_map) {
        float one_pixel = 2.0f / pLwc->height;
        int thickness = 1;
        for (int i = -thickness; i <= thickness; i++) {
            for (int j = -thickness; j <= thickness; j++) {
                render_route_line(pLwc, world_map_x + i * one_pixel, world_map_y + j * one_pixel);
            }
        }
    }
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    render_solid_box_ui_lvt_flip_y_uv(pLwc, 0, 0, 2 * pLwc->aspect_ratio, 2, pLwc->font_fbo.color_tex, LVT_CENTER_CENTER_ANCHORED_SQUARE, 1);
    glEnable(GL_DEPTH_TEST);
}
