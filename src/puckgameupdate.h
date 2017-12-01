#pragma once
typedef struct _LWCONTEXT LWCONTEXT;
typedef struct _LWPUCKGAME LWPUCKGAME;
typedef struct _LWPUCKGAMETOWER LWPUCKGAMETOWER;
typedef struct _LWPUCKGAMEPLAYER LWPUCKGAMEPLAYER;

void update_puck_game(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, double delta_time);
void puck_game_dash(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_target_move(LWPUCKGAME* puck_game, float dx, float dy);
void puck_game_target_stop(LWPUCKGAME* puck_game);
void puck_game_target_dash(LWPUCKGAME* puck_game, int player_no);
void puck_game_pull_puck_start(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_pull_puck_stop(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_pull_puck_toggle(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_rematch(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_reset_view_proj(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_jump(LWCONTEXT* pLwc, LWPUCKGAME* puck_game);
void puck_game_fire(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, float puck_fire_dx, float puck_fire_dy, float puck_fire_dlen);
void puck_game_shake_player(LWPUCKGAME* puck_game, LWPUCKGAMEPLAYER* player);
void puck_game_spawn_tower_damage_text(LWCONTEXT* pLwc, LWPUCKGAME* puck_game, LWPUCKGAMETOWER* tower, int damage);
void puck_game_player_tower_decrease_hp_test(LWPUCKGAME* puck_game);
void puck_game_target_tower_decrease_hp_test(LWPUCKGAME* puck_game);
int puck_game_remote(const LWCONTEXT* pLwc, const LWPUCKGAME* puck_game);