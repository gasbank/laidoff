#pragma once

#include <linmath.h>

typedef struct _LWCONTEXT LWCONTEXT;
typedef struct _LWEMITTER2OBJECT LWEMITTER2OBJECT;

void lwc_render_ps(const LWCONTEXT* pLwc);
void ps_render_explosion(const LWCONTEXT* pLwc, const LWEMITTER2OBJECT* emit_object, const mat4x4 mvp);
