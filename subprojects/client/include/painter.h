#pragma once

#include <client/include/state.h>

namespace space::painter {
    void draw_pixel(state_t&, fun::data::grid_pos_t, fun::rgb_t);
    void draw_line(state_t&, fun::data::grid_pos_t, fun::data::grid_pos_t, fun::rgb_t);
    void draw_bucket(state_t&, fun::data::grid_pos_t, fun::rgb_t, uint32_t);
    void draw_rect(state_t&, fun::data::grid_pos_t, fun::data::grid_pos_t, fun::rgb_t, bool, bool);
}
