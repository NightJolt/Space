#include "packet.h"

#include <FunEngine2D/core/include/color.h>

space::packet_t::packet_t() : pixel_array_size(0) {
    data = fun::bytes_t::create(constants::max_packet_size);
    cursor = data.get_data();
}

void space::packet_t::reset() {
    cursor = data.get_data();
    pixel_array_size = 0;
}

uint8_t* space::packet_t::finalize() {
    *cursor = primitive_type_t::none;
    cursor++;

    return data.get_data();
}

int32_t space::packet_t::bytes_used() {
    return cursor - data.get_data();
}

int32_t space::packet_t::bytes_left() {
    return constants::max_packet_size - bytes_used() - 1;
}

bool space::packet_t::can_fit_pixel_array(uint16_t count) {
    return bytes_left() >= count * 11 + 3 * (last_primitive_type != primitive_type_t::pixel_array);
}

bool space::packet_t::can_fit_line() {
    return bytes_left() >= 20;
}

bool space::packet_t::can_fit_chunk() {
    return bytes_left() >= constants::chunk_size_2d * 3 + 1;
}

void space::packet_t::add_pixel_array(fun::data::grid_pos_t* pos_arr, fun::rgb_t* color_arr, uint16_t count) {
    if (last_primitive_type == primitive_type_t::pixel_array) {
        *(uint16_t*)(cursor - 11 * pixel_array_size) += count;
    } else {
        *cursor = last_primitive_type = primitive_type_t::pixel_array;
        cursor += sizeof(primitive_type_t);

        *(uint16_t*)cursor = count;
        cursor += sizeof(uint16_t);
    }

    pixel_t pixel;
    for (uint16_t i = 0; i < count; i++) {
        pixel.pos = pos_arr[i];
        pixel.color = color_arr[i];

        *(pixel_t*)cursor = pixel;
        cursor += sizeof(pixel_t);
    }
}

void space::packet_t::add_line(fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color) {
    *cursor = last_primitive_type = primitive_type_t::line;
    cursor += sizeof(primitive_type_t);

    line_t line;
    line.start = start;
    line.end = end;
    line.color = color;

    *(line_t*)cursor = line;
    cursor += sizeof(line_t);
}

void space::packet_t::add_chunk(uint8_t* chunk) {
    *cursor = last_primitive_type = primitive_type_t::chunk;
    cursor += sizeof(primitive_type_t);

    memcpy(cursor, chunk, constants::chunk_size_2d * 3);
    cursor += constants::chunk_size_2d * 3;
}

fun::bytes_t& space::packet_t::get_data() {
    return data;
}

space::primitive_type_t space::packet_t::get_primitive_type() {
    return static_cast <primitive_type_t> (*cursor);
}

fun::iterator_t<space::pixel_t> space::packet_t::get_pixel_array_iterator() {
    cursor += sizeof(primitive_type_t);

    uint16_t count = *(uint16_t*)cursor;
    cursor += sizeof(uint16_t);

    pixel_t* begin = (pixel_t*)cursor;
    pixel_t* end = begin + count;
    
    cursor = (uint8_t*)end;

    return { begin, end };
}

space::line_t space::packet_t::get_line() {
    cursor += sizeof(primitive_type_t);

    line_t line = *(line_t*)cursor;
    cursor += sizeof(line_t);

    return line;
}
