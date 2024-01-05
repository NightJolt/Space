#pragma once

#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/data/gridmap/defs.h>
#include <FunEngine2D/core/include/color.h>

namespace space {
    enum primitive_type_t : uint8_t {
        none,
        pixel_array,
        line,
        chunk,
    };

#pragma pack(push, 1)
    struct pixel_t {
        fun::data::grid_pos_t pos;
        fun::rgb_t color;
    };
    
    struct line_t {
        fun::data::grid_pos_t start;
        fun::data::grid_pos_t end;
        fun::rgb_t color;
    };
#pragma pack(pop)
}