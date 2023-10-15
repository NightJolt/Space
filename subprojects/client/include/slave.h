#pragma once

#include <client/include/state.h>

namespace space::slave {
    void send_pixel(state_t&, fun::data::grid_pos_t, fun::rgb_t);
    void send_line(state_t&, fun::data::grid_pos_t, fun::data::grid_pos_t, fun::rgb_t);
    void send_bucket(state_t&, fun::data::grid_pos_t, fun::rgb_t);
    void send_rect(state_t&, fun::data::grid_pos_t, fun::data::grid_pos_t, fun::rgb_t, bool);
}
