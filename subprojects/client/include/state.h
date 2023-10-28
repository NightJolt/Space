#pragma once

#include <FunEngine2D/core/include/render/meshmap/meshmap.h>
#include <FunEngine2D/core/include/render/sprite.h>

#include "tool.h"
#include "connection.h"

namespace space {
    struct state_t {
        fun::render::meshmap_t<128, 1.f> canvas { fun::render::texture_t {}, fun::rgb::black };
        fun::render::meshmap_t<128, 1.f, true> preview_canvas { fun::render::texture_t {}, fun::rgba::transparent };

        tool_t tool;
        fun::render::sprite_t focus_rect;

        connection::connection_data_t connection_data;

        struct {
            bool selecting { false };
            bool selected { false };
            fun::data::grid_pos_t start;
            fun::data::grid_pos_t end;
        } selection;

        struct {
            struct {
                bool last_frame_active { false };
                // fun::vec2f_t last_world_pos;
                fun::data::grid_pos_t last_grid_pos;
            } mouse;
        } data;
    };

    void state_init(state_t&);
    void state_update(state_t&);
    void state_exit(state_t&);
}