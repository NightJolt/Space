#pragma once

#include <FunEngine2D/core/include/render/meshmap/meshmap.h>

#include "tool.h"

namespace space {

struct state_t {
    fun::render::meshmap_t<128, 1.f> canvas { fun::render::texture_t {}, fun::rgb::black };
    fun::render::meshmap_t<128, 1.f, true> preview_canvas { fun::render::texture_t {}, fun::rgba::transparent };

    tool_t tool;

    struct {
        struct {
            bool last_frame_active = false;
            fun::vec2f_t last_world_pos;
            fun::data::grid_pos_t last_grid_pos;
        } mouse;
    } data;
};

}