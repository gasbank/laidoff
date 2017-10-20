#pragma once

typedef struct _LWCONTEXT LWCONTEXT;

void lw_press_key_left(LWCONTEXT* pLwc);
void lw_press_key_right(LWCONTEXT* pLwc);
void lw_press_key_up(LWCONTEXT* pLwc);
void lw_press_key_down(LWCONTEXT* pLwc);
void lw_press_key_space(LWCONTEXT* pLwc);
void lw_release_key_left(LWCONTEXT* pLwc);
void lw_release_key_right(LWCONTEXT* pLwc);
void lw_release_key_up(LWCONTEXT* pLwc);
void lw_release_key_down(LWCONTEXT* pLwc);
void lw_release_key_space(LWCONTEXT* pLwc);
