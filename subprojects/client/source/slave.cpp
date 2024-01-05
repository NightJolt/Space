#include "slave.h"

#include <client/include/painter.h>

void space::slave::send_pixel(state_t& state, fun::data::grid_pos_t pos, fun::rgb_t color) {
    painter::draw_pixel(state, pos, color);

    if (!state.connection_data.packet.can_fit_pixel_array(1)) {
        connection::send_packet(state.connection_data);
    }
    
    state.connection_data.packet.add_pixel_array(&pos, &color, 1);
}

void space::slave::send_line(state_t& state, fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color) {
    painter::draw_line(state, start, end, color);

    if (!state.connection_data.packet.can_fit_line()) {
        connection::send_packet(state.connection_data);
    }
    
    state.connection_data.packet.add_line(start, end, color);
}

void space::slave::send_bucket(state_t& state, fun::data::grid_pos_t pos, fun::rgb_t color) {
    painter::draw_bucket(state, pos, color, 10000);
}

void space::slave::send_rect(state_t& state, fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color, bool filled) {
    painter::draw_rect(state, start, end, color, filled, false);
}

void space::slave::process_packet(state_t& state, packet_t& packet) {
    while (true) {
        primitive_type_t type = packet.get_primitive_type();

        if (type == primitive_type_t::none) {
            break;
        }

        switch (type) {
            case primitive_type_t::pixel_array: {
                for (auto pixel : packet.get_pixel_array_iterator()) {
                    painter::draw_pixel(state, pixel.pos, pixel.color);
                }

                break;
            }

            case primitive_type_t::line: {
                auto line = packet.get_line();
                painter::draw_line(state, line.start, line.end, line.color);

                break;
            }

            case primitive_type_t::chunk: {
                break;
            }

            case primitive_type_t::none: {
                break;
            }
        }
    }
};