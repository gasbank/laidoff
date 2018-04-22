#pragma once

#ifdef __cplusplus
extern "C" {;
#endif
typedef struct _LWCONTEXT LWCONTEXT;
typedef struct _LWUDP LWUDP;

typedef struct _LWTTLLNGLAT {
    float lng;
    float lat;
} LWTTLLNGLAT;

typedef struct _LWTTLWORLDMAP {
    float render_org_x;
    float render_org_y;
    LWTTLLNGLAT center;
    float zoom_scale;
} LWTTLWORLDMAP;

void* lwttl_new(float aspect_ratio);
void lwttl_destroy(void** __ttl);
void lwttl_render_all_seaports(const LWCONTEXT* pLwc, const void* _ttl, const LWTTLWORLDMAP* worldmap);
float lnglat_to_xy(const LWCONTEXT* pLwc, float v);
void lwttl_worldmap_scroll(void* _ttl, float dlng, float dlat, float dzoom);
void lwttl_worldmap_scroll_to(void* _ttl, float lng, float lat, LWUDP* udp_sea);
const LWTTLWORLDMAP* lwttl_worldmap(void* _ttl);
void lwttl_update_aspect_ratio(void* _ttl, float aspect_ratio);
const LWTTLLNGLAT* lwttl_center(const void* _ttl);
void lwttl_update(LWCONTEXT* pLwc, void* _ttl, float delta_time);
const char* lwttl_http_header(const void* _ttl);
int lwttl_track_object_id(const void* _ttl);
void lwttl_set_track_object_id(const void* _ttl, int v);
int lwttl_track_object_ship_id(const void* _ttl);
void lwttl_set_track_object_ship_id(const void* _ttl, int v);
void lwttl_set_center(void* _ttl, float lng, float lat);
void lwttl_set_seaarea(void* _ttl, const char* name);
const char* lwttl_seaarea(void* _ttl);
void lwttl_set_view_scale(const void* _ttl, int v);
int lwttl_view_scale(const void* _ttl);
void lwttl_udp_send_ttlping(const void* ttl, LWUDP* udp, int ping_seq);
void lwttl_set_xc0(void* _ttl, int v);
int lwttl_xc0(const void* _ttl);
void lwttl_set_yc0(void* _ttl, int v);
int lwttl_yc0(const void* _ttl);
void lwttl_lock_rendering_mutex(void* _ttl);
void lwttl_unlock_rendering_mutex(void* _ttl);
#ifdef __cplusplus
}
#endif
