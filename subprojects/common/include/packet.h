#pragma once

#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/data/gridmap/defs.h>

#include <common/include/command.h>

namespace space {
    struct packet_t {
        void reset();

        uint32_t bytes_used();
        uint32_t bytes_left();

        void add_pixel_array(fun::data::grid_pos_t*, fun::rgb_t*, uint16_t);
        void add_line(fun::data::grid_pos_t, fun::data::grid_pos_t, fun::rgb_t);
        
        uint8_t* data { nullptr };
        uint8_t* cursor { nullptr };
    };
}
