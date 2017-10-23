#include "lwlog.h"
#include "lwcontext.h"
#include "battle.h"
#include "laidoff.h"
#include "render_admin.h"
#include "battle_result.h"
#include "field.h"
#include "mq.h"
#include "logic.h"
#include "lwbutton.h"
#include "puckgameupdate.h"

float get_dir_pad_size_radius() {
	return 0.75f;
}

void get_dir_pad_center(float aspect_ratio, float *x, float *y) {
	const float sr = get_dir_pad_size_radius();
	if (aspect_ratio > 1) {
		*x = -1 * aspect_ratio + sr;
		*y = -1 + sr;
	} else {
		*x = -1 + sr;
		*y = -1 / aspect_ratio + sr;
	}
}

int lw_get_normalized_dir_pad_input(const LWCONTEXT* pLwc, float *dx, float *dy, float *dlen) {
	if (!pLwc->dir_pad_dragging) {
		return 0;
	}

	const float aspect_ratio = (float)pLwc->width / pLwc->height;
	float dir_pad_center_x = 0;
	float dir_pad_center_y = 0;
	get_dir_pad_center(aspect_ratio, &dir_pad_center_x, &dir_pad_center_y);

	*dx = pLwc->dir_pad_x - dir_pad_center_x;
	*dy = pLwc->dir_pad_y - dir_pad_center_y;

	*dlen = sqrtf(*dx * *dx + *dy * *dy);
	
	if (*dlen < LWEPSILON) {
		*dlen = 0;
		*dx = 0;
		*dy = 0;
	} else {
		*dx /= *dlen;
		*dy /= *dlen;
	}

	return 1;
}

void reset_dir_pad_position(LWCONTEXT* pLwc) {
	const float aspect_ratio = (float)pLwc->width / pLwc->height;

	float dir_pad_center_x = 0;
	float dir_pad_center_y = 0;
	get_dir_pad_center(aspect_ratio, &dir_pad_center_x, &dir_pad_center_y);

	pLwc->dir_pad_x = dir_pad_center_x;
	pLwc->dir_pad_y = dir_pad_center_y;
}

static void convert_touch_coord_to_ui_coord(LWCONTEXT* pLwc, float *x, float *y) {
	if (pLwc->height < pLwc->width) {
		*x *= (float)pLwc->width / pLwc->height;
	} else {
		*y *= (float)pLwc->height / pLwc->width;
	}
}

void lw_trigger_mouse_press(LWCONTEXT* pLwc, float x, float y, int pointer_id) {
	if (!pLwc) {
		return;
	}

	convert_touch_coord_to_ui_coord(pLwc, &x, &y);

	//LOGI("mouse press ui coord x=%f, y=%f\n", x, y);

	if (field_network(pLwc->field)) {
		mq_publish_now(pLwc, pLwc->mq, 0);
	}

	pLwc->last_mouse_press_x = x;
	pLwc->last_mouse_press_y = y;

	const float aspect_ratio = (float)pLwc->width / pLwc->height;

	float dir_pad_center_x = 0;
	float dir_pad_center_y = 0;
	get_dir_pad_center(aspect_ratio, &dir_pad_center_x, &dir_pad_center_y);

	const float sr = get_dir_pad_size_radius();

	if ((pLwc->game_scene == LGS_FIELD || pLwc->game_scene == LGS_PHYSICS)
		&& fabs(dir_pad_center_x - x) < sr && fabs(dir_pad_center_y - y) < sr
		&& !pLwc->dir_pad_dragging) {
		pLwc->dir_pad_x = x;
		pLwc->dir_pad_y = y;
		pLwc->dir_pad_dragging = 1;
		pLwc->dir_pad_pointer_id = pointer_id;
	}

	if (pLwc->game_scene == LGS_PHYSICS && fabs(aspect_ratio - 0.3f - 0.75f / 2 - x) < 0.75f && fabs(-1 + 0.75f / 2 - y) < 0.75f
		&& (!pLwc->dir_pad_dragging || (pLwc->dir_pad_pointer_id != pointer_id))) {
		puck_game_dash(pLwc, pLwc->puck_game);
	}

	if (pLwc->game_scene == LGS_FIELD && fabs(aspect_ratio - 0.3f - 0.75f/2 - x) < 0.75f && fabs(-1 + 0.75f/2 - y) < 0.75f) {
		//field_attack(pLwc);

		//pLwc->hide_field = !pLwc->hide_field;
		
		//LOGI("atk_pad_dragging ON");

		// Player combat mode...
		//pLwc->atk_pad_dragging = 1;
	}



	if (pLwc->game_scene == LGS_FIELD && fabs(aspect_ratio - 0.3f - 0.75f / 2 - x) < 0.75f && fabs(1 - 0.75f / 2 - y) < 0.75f) {
		//pLwc->fps_mode = !pLwc->fps_mode;
	}

	if (pLwc->game_scene == LGS_BATTLE && pLwc->battle_state != LBS_COMMAND_IN_PROGRESS && pLwc->player_turn_creature_index >= 0) {
		const float command_palette_pos = -0.5f;

		if (y > command_palette_pos) {
			exec_attack_p2e_with_screen_point(pLwc, x, y);
		} else {
			// command palette area
			int command_slot = (int)((x + aspect_ratio) / (2.0f / 10 * aspect_ratio)) - 2;
			if (command_slot >= 0 && command_slot < 6) {
				const LWSKILL* skill = pLwc->player[pLwc->player_turn_creature_index].skill[command_slot];
				if (skill && skill->valid) {
					pLwc->selected_command_slot = command_slot;
				}
			}

			//printf("mouse press command slot %d\n", command_slot);
		}
	}
	int pressed_idx = lwbutton_press(&pLwc->button_list, x, y);
	if (pressed_idx >= 0) {
		const char* id = lwbutton_id(&pLwc->button_list, pressed_idx);
		logic_emit_ui_event_async(pLwc, id);
	}
}

void lw_trigger_mouse_move(LWCONTEXT* pLwc, float x, float y, int pointer_id) {
	if (!pLwc) {
		return;
	}

	convert_touch_coord_to_ui_coord(pLwc, &x, &y);

	pLwc->last_mouse_move_delta_x = x - pLwc->last_mouse_move_x;
	pLwc->last_mouse_move_delta_y = y - pLwc->last_mouse_move_y;

	pLwc->last_mouse_move_x = x;
	pLwc->last_mouse_move_y = y;

	const float aspect_ratio = (float)pLwc->width / pLwc->height;

	if ((pLwc->game_scene == LGS_FIELD || pLwc->game_scene == LGS_PHYSICS)
		&& pLwc->dir_pad_dragging && pLwc->dir_pad_pointer_id == pointer_id) {
		float dir_pad_center_x = 0;
		float dir_pad_center_y = 0;
		get_dir_pad_center(aspect_ratio, &dir_pad_center_x, &dir_pad_center_y);

		const float sr = get_dir_pad_size_radius();

		if (x < dir_pad_center_x - sr) {
			x = dir_pad_center_x - sr;
		}

		if (x > dir_pad_center_x + sr) {
			x = dir_pad_center_x + sr;
		}

		if (y < dir_pad_center_y - sr) {
			y = dir_pad_center_y - sr;
		}

		if (y > dir_pad_center_y + sr) {
			y = dir_pad_center_y + sr;
		}

		pLwc->dir_pad_x = x;
		pLwc->dir_pad_y = y;
	}
}

void lw_trigger_mouse_release(LWCONTEXT* pLwc, float x, float y, int pointer_id) {
	if (!pLwc) {
		return;
	}

	convert_touch_coord_to_ui_coord(pLwc, &x, &y);

	//printf("mouse release ui coord x=%f, y=%f (last move ui coord x=%f, y=%f)\n", x, y, pLwc->last_mouse_press_x, pLwc->last_mouse_press_y);

	if (field_network(pLwc->field)) {
		mq_publish_now(pLwc, pLwc->mq, 1);
	}

	const float aspect_ratio = (float)pLwc->width / pLwc->height;

	// Touch left top corner of the screen
	if (pLwc->game_scene != LGS_ADMIN
		&& x < -aspect_ratio + 0.25f
		&& y > 1.0f - 0.25f) {

		change_to_admin(pLwc);
		return;
	}

	// Touch right top corner of the screen
	if (pLwc->game_scene != LGS_ADMIN
		&& x > aspect_ratio - 0.25f
		&& y > 1.0f - 0.25f) {

		reset_time(pLwc);
		return;
	}

	if (pLwc->dir_pad_pointer_id == pointer_id) {
		reset_dir_pad_position(pLwc);
		pLwc->dir_pad_dragging = 0;
	}

	pLwc->atk_pad_dragging = 0;
	//LOGI("atk_pad_dragging OFF");

	if (pLwc->game_scene == LGS_ADMIN) {
		touch_admin(pLwc, pLwc->last_mouse_press_x, pLwc->last_mouse_press_y, x, y);
	}
}

void lw_trigger_touch(LWCONTEXT* pLwc, float x, float y, int pointer_id) {
	if (!pLwc) {
		return;
	}

	convert_touch_coord_to_ui_coord(pLwc, &x, &y);

	pLwc->dialog_move_next = 1;

	if (pLwc->game_scene == LGS_FIELD) {

	} else if (pLwc->game_scene == LGS_DIALOG) {

	} else if (pLwc->game_scene == LGS_BATTLE) {

	} else if (pLwc->game_scene == LGS_ADMIN) {

	} else if (pLwc->game_scene == LGS_BATTLE_RESULT) {
		process_touch_battle_result(pLwc, x, y);
	}
}

void lw_trigger_reset(LWCONTEXT* pLwc) {
	reset_runtime_context_async(pLwc);
}

void lw_trigger_play_sound(LWCONTEXT* pLwc) {
}

void lw_trigger_key_right(LWCONTEXT* pLwc) {

	// battle

	if (pLwc->battle_state == LBS_SELECT_COMMAND) {
		if (pLwc->selected_command_slot != -1 && pLwc->player_turn_creature_index >= 0) {
			int new_selected_command_slot = -1;
			for (int i = pLwc->selected_command_slot + 1; i < MAX_COMMAND_SLOT; i++) {
				const LWSKILL* s = pLwc->player[pLwc->player_turn_creature_index].skill[i];
				if (s && s->valid) {

					new_selected_command_slot = i;
					break;
				}
			}

			if (new_selected_command_slot != -1) {
				pLwc->selected_command_slot = new_selected_command_slot;
			}
		}
	} else if (pLwc->battle_state == LBS_SELECT_TARGET) {

		if (pLwc->selected_enemy_slot != -1) {
			int new_selected_enemy_slot = -1;
			for (int i = pLwc->selected_enemy_slot + 1; i < MAX_ENEMY_SLOT; i++) {
				if (pLwc->enemy[i].valid
					&& pLwc->enemy[i].c.hp > 0) {

					new_selected_enemy_slot = i;
					break;
				}
			}

			if (new_selected_enemy_slot != -1) {
				pLwc->selected_enemy_slot = new_selected_enemy_slot;
			}
		}
	}
}

void lw_trigger_key_left(LWCONTEXT* pLwc) {

	// battle

	if (pLwc->battle_state == LBS_SELECT_COMMAND) {
		if (pLwc->selected_command_slot != -1 && pLwc->player_turn_creature_index >= 0) {
			int new_selected_command_slot = -1;
			for (int i = pLwc->selected_command_slot - 1; i >= 0; i--) {
				const LWSKILL* s = pLwc->player[pLwc->player_turn_creature_index].skill[i];
				if (s && s->valid) {

					new_selected_command_slot = i;
					break;
				}
			}

			if (new_selected_command_slot != -1) {
				pLwc->selected_command_slot = new_selected_command_slot;
			}
		}
	} else if (pLwc->battle_state == LBS_SELECT_TARGET) {
		if (pLwc->selected_enemy_slot != -1) {
			int new_selected_enemy_slot = -1;
			for (int i = pLwc->selected_enemy_slot - 1; i >= 0; i--) {
				if (pLwc->enemy[i].valid
					&& pLwc->enemy[i].c.hp > 0) {

					new_selected_enemy_slot = i;
					break;
				}
			}

			if (new_selected_enemy_slot != -1) {
				pLwc->selected_enemy_slot = new_selected_enemy_slot;
			}
		}
	}
}

void lw_trigger_key_enter(LWCONTEXT* pLwc) {

	// battle

	if (pLwc->battle_state == LBS_SELECT_COMMAND && pLwc->player_turn_creature_index >= 0) {
		pLwc->battle_state = LBS_SELECT_TARGET;

		for (int i = 0; i < ARRAY_SIZE(pLwc->enemy); i++) {
			if (pLwc->enemy[i].valid && pLwc->enemy[i].c.hp > 0) {
				pLwc->selected_enemy_slot = i;
				break;
			}
		}
	} else if (pLwc->battle_state == LBS_SELECT_TARGET) {

		exec_attack_p2e(pLwc, pLwc->selected_enemy_slot);
	}
}

static void simulate_dir_pad_touch_input(LWCONTEXT* pLwc) {
	pLwc->dir_pad_dragging = pLwc->player_move_left || pLwc->player_move_right || pLwc->player_move_down || pLwc->player_move_up;

	const float aspect_ratio = (float)pLwc->width / pLwc->height;
	float dir_pad_center_x = 0;
	float dir_pad_center_y = 0;
	get_dir_pad_center(aspect_ratio, &dir_pad_center_x, &dir_pad_center_y);

	pLwc->dir_pad_x = dir_pad_center_x + (pLwc->player_move_right - pLwc->player_move_left) / 5.0f;
	pLwc->dir_pad_y = dir_pad_center_y + (pLwc->player_move_up - pLwc->player_move_down) / 5.0f;
}

void lw_press_key_left(LWCONTEXT* pLwc) {
	pLwc->player_move_left = 1;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_press_key_right(LWCONTEXT* pLwc) {
	pLwc->player_move_right = 1;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_press_key_up(LWCONTEXT* pLwc) {
	pLwc->player_move_up = 1;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_press_key_down(LWCONTEXT* pLwc) {
	pLwc->player_move_down = 1;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_press_key_space(LWCONTEXT* pLwc) {
	pLwc->player_space = 1;
}

void lw_press_key_z(LWCONTEXT* pLwc) {
	puck_game_dash(pLwc, pLwc->puck_game);
}

void lw_release_key_left(LWCONTEXT* pLwc) {
	pLwc->player_move_left = 0;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_release_key_right(LWCONTEXT* pLwc) {
	pLwc->player_move_right = 0;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_release_key_up(LWCONTEXT* pLwc) {
	pLwc->player_move_up = 0;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_release_key_down(LWCONTEXT* pLwc) {
	pLwc->player_move_down = 0;
	simulate_dir_pad_touch_input(pLwc);
}

void lw_release_key_space(LWCONTEXT* pLwc) {
	pLwc->player_space = 0;
}

void lw_release_key_z(LWCONTEXT* pLwc) {
}
