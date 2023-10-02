#pragma once

#include "canvas.h"
#include "tool.h"

namespace space {

struct state_t {
    canvas_t canvas;
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