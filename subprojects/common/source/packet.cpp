#include "packet.h"

#include <FunEngine2D/core/include/color.h>

namespace {
    static constexpr uint32_t max_packet_size { 1024 };
}

void space::packet_t::reset() {
    delete[] data;
    
    data = new uint8_t[max_packet_size];
    cursor = data;
}

uint32_t space::packet_t::bytes_used() {
    return cursor - data;
}

uint32_t space::packet_t::bytes_left() {
    return max_packet_size - bytes_used();
}

// * 1 byte - cmd, 2 byte - count, count * 11 byte
void space::packet_t::add_pixel_array(fun::data::grid_pos_t* pos_arr, fun::rgb_t* color_arr, uint16_t count) {
    *cursor = command_type_t::pixel_array;
    cursor++;

    *reinterpret_cast <uint16_t*> (cursor) = count;
    cursor += 2;

    for (uint16_t i = 0; i < count; i++) {
        *reinterpret_cast <fun::data::grid_pos_t*> (cursor) = pos_arr[i];
        cursor += 8;

        *reinterpret_cast <fun::rgb_t*> (cursor) = color_arr[i];
        cursor += 3;
    }
}

// * 1 byte - cmd, 8 byte - start, 8 byte - end, 3 byte - color
void space::packet_t::add_line(fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color) {
    *cursor = command_type_t::line;
    cursor++;

    *reinterpret_cast <fun::data::grid_pos_t*> (cursor) = start;
    cursor += 8;

    *reinterpret_cast <fun::data::grid_pos_t*> (cursor) = end;
    cursor += 8;

    *reinterpret_cast <fun::rgb_t*> (cursor) = color;
    cursor += 3;
}
