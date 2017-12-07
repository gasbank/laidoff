#include "lwbutton.h"
#include "lwmacro.h"
#include "lwlog.h"
#include <string.h>

LWBUTTON* lwbutton_lae_append(LWBUTTONLIST* button_list, const char* id, float x, float y, float w, float h,
                              LW_ATLAS_ENUM lae, LW_ATLAS_ENUM lae_alpha, float ui_alpha) {
    LWBUTTON* b = lwbutton_append(button_list, id, x, y, w, h);
    b->lae = lae;
    b->lae_alpha = lae_alpha;
    b->ui_alpha = ui_alpha;
    return b;
}

LWBUTTON* lwbutton_append(LWBUTTONLIST* button_list, const char* id, float x, float y, float w, float h) {
    if (button_list->button_count >= ARRAY_SIZE(button_list->button)) {
        LOGE(LWLOGPOS "ARRAY_SIZE(button_list->button) exceeded");
        return 0;
    }
    LWBUTTON* b = &button_list->button[button_list->button_count];
    if (strlen(id) > ARRAY_SIZE(b->id) - 1) {
        LOGE(LWLOGPOS "ARRAY_SIZE(b->id) exceeded");
        return 0;
    }
    strcpy(b->id, id);
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    button_list->button_count++;
    return b;
}

int lwbutton_press(const LWBUTTONLIST* button_list, float x, float y) {
    if (button_list->button_count >= ARRAY_SIZE(button_list->button)) {
        LOGE(LWLOGPOS "ARRAY_SIZE(button_list->button) exceeded");
        return -1;
    }
    for (int i = 0; i < button_list->button_count; i++) {
        const LWBUTTON* b = &button_list->button[i];
        if (b->x <= x && x <= b->x + b->w && b->y - b->h <= y && y <= b->y) {
            return i;
        }
    }
    return -1;
}

const char* lwbutton_id(const LWBUTTONLIST* button_list, int idx) {
    if (idx >= ARRAY_SIZE(button_list->button)) {
        LOGE(LWLOGPOS "ARRAY_SIZE(button_list->button) exceeded");
        return 0;
    }
    return button_list->button[idx].id;
}
