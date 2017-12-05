#include "platform_detection.h"
#if LW_PLATFORM_WIN32
#include <WinSock2.h>
#endif
#include "puckgame.h"
#include "lwmacro.h"
#include "lwlog.h"
#include "lwcontext.h"
#include "input.h"
#include "lwudp.h"
#include "puckgamepacket.h"
#include "lwtcp.h"
#include "puckgameupdate.h"
#include "lwtcpclient.h"
#include "lwmath.h"

void update_world_roll(LWPUCKGAME* puck_game) {
    if (puck_game->world_roll_dirty) {
        const float world_roll_diff = puck_game->world_roll_target - puck_game->world_roll;
        if (fabsf(world_roll_diff) < LWEPSILON) {
            puck_game->world_roll = puck_game->world_roll_target;
            puck_game->world_roll_dirty = 0;
            LOGI("World roll transition finished");
        } else {
            puck_game->world_roll += world_roll_diff * puck_game->world_roll_target_follow_ratio;
        }
    }
}

void update_shake(LWPUCKGAME* puck_game, float delta_time) {
    for (int i = 0; i < 2; i++) {
        // Decrease shake remain time
        if (puck_game->remote_dash[i].shake_remain_time > 0) {
            puck_game->remote_dash[i].shake_remain_time = LWMAX(0, puck_game->remote_dash[i].shake_remain_time - (float)delta_time);
        }
        // Jump
        if (puck_game->remote_jump[i].shake_remain_time > 0) {
            puck_game->remote_jump[i].shake_remain_time = LWMAX(0, puck_game->remote_jump[i].shake_remain_time - (float)delta_time);
        }
    }
    // Decrease HP remain time (player)
    if (puck_game->player.hp_shake_remain_time > 0) {
        puck_game->player.hp_shake_remain_time = LWMAX(0, puck_game->player.hp_shake_remain_time - (float)delta_time);
    }
    // Decrease HP remain time (target)
    if (puck_game->target.hp_shake_remain_time > 0) {
        puck_game->target.hp_shake_remain_time = LWMAX(0, puck_game->target.hp_shake_remain_time - (float)delta_time);
    }
    // Tower shake
    for (int i = 0; i < LW_PUCK_GAME_TOWER_COUNT; i++) {
        if (puck_game->tower[i].shake_remain_time > 0) {
            puck_game->tower[i].shake_remain_time = LWMAX(0, puck_game->tower[i].shake_remain_time - (float)delta_time);
        }
    }
}

void update_puck_game(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, double delta_time) {
    if (!puck_game->world) {
        return;
    }
    const int remote = puck_game_remote(pLwc, puck_game);
    puck_game->on_player_damaged = remote ? 0 : puck_game_player_tower_decrease_hp_test;
    puck_game->on_target_damaged = remote ? 0 : puck_game_target_tower_decrease_hp_test;
    puck_game->time += (float)delta_time;
    puck_game->player.puck_contacted = 0;
    if (puck_game->battle_id == 0) {
        dSpaceCollide(puck_game->space, puck_game, puck_game_near_callback);
        //dWorldStep(puck_game->world, 0.005f);
        dWorldQuickStep(puck_game->world, 1.0f / 60);
        dJointGroupEmpty(puck_game->contact_joint_group);
    }
    if (puck_game->player.puck_contacted == 0) {
        puck_game->player.last_contact_puck_body = 0;
    }

    float dx, dy, dlen;
    int dir_pad_dragging = lw_get_normalized_dir_pad_input(pLwc, &pLwc->left_dir_pad, &dx, &dy, &dlen);
    const float dlen_max = pLwc->left_dir_pad.max_follow_distance;
    if (dlen > dlen_max) {
        dlen = dlen_max;
    }
    float dlen_ratio = dlen / dlen_max;
    // make dlen_ratio ^ 4
    dlen_ratio *= dlen_ratio;
    dlen_ratio *= dlen_ratio;
    puck_game->remote_control[0].dir_pad_dragging = pLwc->left_dir_pad.dragging;
    puck_game->remote_control[0].dx = dx;
    puck_game->remote_control[0].dy = dy;
    puck_game->remote_control[0].dlen = dlen_ratio;

    const int send_udp = remote && pLwc->puck_game_state.bf.finished == 0;
    
    if (dir_pad_dragging) {
        LOGIx("dx=%.2f, dy=%.2f, dlen=%.2f, dlen_max=%.2f", dx, dy, dlen, dlen_max);
        if (send_udp) {
            LWPMOVE packet_move;
            packet_move.type = LPGP_LWPMOVE;
            packet_move.battle_id = pLwc->puck_game->battle_id;
            packet_move.token = pLwc->puck_game->token;
            packet_move.dx = pLwc->puck_game->player_no == 2 ? -dx : dx;
            packet_move.dy = pLwc->puck_game->player_no == 2 ? -dy : dy;
            packet_move.dlen = dlen_ratio;
            udp_send(pLwc->udp, (const char*)&packet_move, sizeof(packet_move));
        }
    } else {
        if (send_udp) {
            LWPSTOP packet_stop;
            packet_stop.type = LPGP_LWPSTOP;
            packet_stop.battle_id = pLwc->puck_game->battle_id;
            packet_stop.token = pLwc->puck_game->token;
            udp_send(pLwc->udp, (const char*)&packet_stop, sizeof(packet_stop));
        }
    }

    // Pull
    if (puck_game->remote_control[0].pull_puck) {
        if (send_udp) {
            LWPPULLSTART p;
            p.type = LPGP_LWPPULLSTART;
            p.battle_id = pLwc->puck_game->battle_id;
            p.token = pLwc->puck_game->token;
            udp_send(pLwc->udp, (const char*)&p, sizeof(p));
        }
    } else {
        if (send_udp) {
            LWPPULLSTOP p;
            p.type = LPGP_LWPPULLSTOP;
            p.battle_id = pLwc->puck_game->battle_id;
            p.token = pLwc->puck_game->token;
            udp_send(pLwc->udp, (const char*)&p, sizeof(p));
        }
    }

    // control bogus (AI player)
    puck_game_control_bogus(puck_game);

    for (int i = 0; i < 2; i++) {
        puck_game_update_remote_player(puck_game, (float)delta_time, i);
    }

    // -------- Client only --------

    update_shake(puck_game, (float)delta_time);
    update_puck_ownership(puck_game);
    update_puck_reflect_size(puck_game, (float)delta_time);
    update_world_roll(puck_game);
}

void puck_game_jump(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    // Check params
    if (!pLwc || !puck_game) {
        return;
    }
    // Check already effective jump
    if (puck_game_jumping(&puck_game->remote_jump[0])) {
        return;
    }
    // Check effective move input
    //float dx, dy, dlen;
    /*if (!lw_get_normalized_dir_pad_input(pLwc, &dx, &dy, &dlen)) {
    return;
    }*/

    // Check cooltime
    if (puck_game_jump_cooltime(puck_game) < puck_game->jump_interval) {
        puck_game->remote_jump[0].shake_remain_time = puck_game->jump_shake_time;
        return;
    }

    // Start jump!
    puck_game_commit_jump(puck_game, &puck_game->remote_jump[0], 1);

    const int remote = puck_game_remote(pLwc, puck_game);

    if (pLwc->puck_game_state.bf.finished == 0
        && remote) {
        LWPJUMP packet_jump;
        packet_jump.type = LPGP_LWPJUMP;
        packet_jump.battle_id = pLwc->puck_game->battle_id;
        packet_jump.token = pLwc->puck_game->token;
        udp_send(pLwc->udp, (const char*)&packet_jump, sizeof(packet_jump));
    }
}

void puck_game_dash_and_send(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    LWPUCKGAMEDASH* dash = &puck_game->remote_dash[puck_game->player_no == 2 ? 1 : 0];
    if (puck_game_dash(puck_game, dash, puck_game->player_no == 2 ? 2 : 1) == 0) {
        const int remote = puck_game_remote(pLwc, puck_game);
        const int send_udp = remote && pLwc->puck_game_state.bf.finished == 0;
        if (send_udp) {
            LWPDASH packet_dash;
            packet_dash.type = LPGP_LWPDASH;
            packet_dash.battle_id = pLwc->puck_game->battle_id;
            packet_dash.token = pLwc->puck_game->token;
            udp_send(pLwc->udp, (const char*)&packet_dash, sizeof(packet_dash));
        }
    }
}

void puck_game_target_move(LWPUCKGAME* puck_game, float dx, float dy) {
    dJointID tcj = puck_game->target_control_joint;
    float player_speed = 0.5f;
    dJointSetLMotorParam(tcj, dParamVel1, player_speed * dx);
    dJointSetLMotorParam(tcj, dParamVel2, player_speed * dy);
    puck_game->last_remote_dx = dx;
    puck_game->last_remote_dy = dy;
}

void puck_game_target_stop(LWPUCKGAME* puck_game) {
    dJointID tcj = puck_game->target_control_joint;
    float player_speed = 0.5f;
    dJointSetLMotorParam(tcj, dParamVel1, 0);
    dJointSetLMotorParam(tcj, dParamVel2, 0);
}

void puck_game_target_dash(LWPUCKGAME* puck_game, int player_no) {
    puck_game_commit_dash(puck_game, &puck_game->remote_dash[player_no - 1], puck_game->last_remote_dx, puck_game->last_remote_dy);
}

void puck_game_pull_puck_start(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    puck_game->remote_control[0].pull_puck = 1;
}

void puck_game_pull_puck_stop(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    puck_game->remote_control[0].pull_puck = 0;
}

void puck_game_pull_puck_toggle(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    if (puck_game->remote_control[0].pull_puck) {
        puck_game_pull_puck_stop(pLwc, puck_game);
    } else {
        puck_game_pull_puck_start(pLwc, puck_game);
    }
}

void puck_game_rematch(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    // queue again after game ended
    if (puck_game->battle_id
        && puck_game->token
        && pLwc->puck_game_state.bf.finished) {
        puck_game->battle_id = 0;
        puck_game->token = 0;
        puck_game->player_no = 1;
        memset(&pLwc->puck_game_state, 0, sizeof(pLwc->puck_game_state));
        puck_game_reset_view_proj(pLwc, puck_game);
        puck_game_reset(puck_game);
        puck_game_remote_state_reset(puck_game, &pLwc->puck_game_state);
        int send_result = tcp_send_queue2(pLwc->tcp, &pLwc->tcp->user_id);
        if (send_result < 0) {
            LOGE(LWLOGPOS "Send result failed: %d", send_result);
        }
    }
}

void puck_game_reset_view_proj(LWCONTEXT* pLwc, LWPUCKGAME* puck_game) {
    // Setup puck game view, proj matrices
    
    mat4x4_perspective(pLwc->puck_game_proj, (float)(LWDEG2RAD(49.1343) / pLwc->aspect_ratio), pLwc->aspect_ratio, 1.0f, 500.0f);
    vec3 eye = { 0.0f, 0.0f, 10.0f /*12.0f*/ };
    vec3 center = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };
    if (puck_game->player_no == 2) {
        up[1] = -1.0f;
    }
    mat4x4_look_at(pLwc->puck_game_view, eye, center, up);
}

void puck_game_fire(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, float puck_fire_dx, float puck_fire_dy, float puck_fire_dlen) {
    puck_game_commit_fire(puck_game, &puck_game->remote_fire[0], 1, puck_fire_dx, puck_fire_dy, puck_fire_dlen);
    LWPFIRE packet_fire;
    packet_fire.type = LPGP_LWPFIRE;
    packet_fire.battle_id = pLwc->puck_game->battle_id;
    packet_fire.token = pLwc->puck_game->token;
    packet_fire.dx = puck_fire_dx;
    packet_fire.dy = puck_fire_dy;
    packet_fire.dlen = puck_fire_dlen;
    udp_send(pLwc->udp, (const char*)&packet_fire, sizeof(packet_fire));
}

void puck_game_shake_player(LWPUCKGAME* puck_game, LWPUCKGAMEPLAYER* player) {
    player->hp_shake_remain_time = puck_game->hp_shake_time;
}

void puck_game_spawn_tower_damage_text(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, LWPUCKGAMETOWER* tower, int damage) {
    tower->shake_remain_time = puck_game->tower_shake_time;
    mat4x4 proj_view;
    mat4x4_identity(proj_view);
    mat4x4_mul(proj_view, pLwc->puck_game_proj, pLwc->puck_game_view);
    vec2 ui_point;
    vec4 tower_world_point;
    puck_game_tower_pos(tower_world_point, puck_game, tower->owner_player_no);
    calculate_ui_point_from_world_point(pLwc->aspect_ratio, proj_view, tower_world_point, ui_point);
    char damage_str[16];
    sprintf(damage_str, "%d", damage);
    spawn_damage_text(pLwc, ui_point[0], ui_point[1], 0, damage_str, LDTC_UI);
}

void puck_game_tower_damage_test(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, LWPUCKGAMEPLAYER* player, LWPUCKGAMETOWER* tower, int damage) {
    puck_game_shake_player(pLwc->puck_game, player);
    puck_game_spawn_tower_damage_text(pLwc, pLwc->puck_game, tower, damage);
}

void puck_game_player_tower_decrease_hp_test(LWPUCKGAME* puck_game) {
    puck_game_tower_damage_test(puck_game->pLwc, puck_game, &puck_game->player, &puck_game->tower[0], 1);
}

void puck_game_target_tower_decrease_hp_test(LWPUCKGAME* puck_game) {
    puck_game_tower_damage_test(puck_game->pLwc, puck_game, &puck_game->target, &puck_game->tower[1], 1);
}

int puck_game_remote(const LWCONTEXT* pLwc, const LWPUCKGAME* puck_game) {
    return puck_game->battle_id != 0;
}
