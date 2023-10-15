#include "slave.h"

#include <client/include/painter.h>

void space::slave::send_pixel(state_t& state, fun::data::grid_pos_t pos, fun::rgb_t color) {
    painter::draw_pixel(state, pos, color);

    // todo: networking stuff
}

void space::slave::send_line(state_t& state, fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color) {
    painter::draw_line(state, start, end, color);
}

void space::slave::send_bucket(state_t& state, fun::data::grid_pos_t pos, fun::rgb_t color) {
    painter::draw_bucket(state, pos, color, 10000);
}

void space::slave::send_rect(state_t& state, fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color, bool filled) {
    painter::draw_rect(state, start, end, color, filled, false);
}
