#pragma once

#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/data/gridmap/defs.h>
#include <FunEngine2D/core/include/iterator.h>
#include <FunEngine2D/core/include/bytes.h>

#include <common/include/primitive.h>

#include "defs.h"

namespace space {
    class packet_t {
    public:
        packet_t();

        void reset();
        uint8_t* finalize();

        int32_t bytes_used();
        int32_t bytes_left();

        bool can_fit_pixel_array(uint16_t);
        bool can_fit_line();
        bool can_fit_chunk();

        void add_pixel_array(fun::data::grid_pos_t*, fun::rgb_t*, uint16_t);
        void add_line(fun::data::grid_pos_t, fun::data::grid_pos_t, fun::rgb_t);
        void add_chunk(uint8_t*);

        fun::bytes_t& get_data();

        primitive_type_t get_primitive_type();

        fun::iterator_t<pixel_t> get_pixel_array_iterator();
        line_t get_line();

    private:
        fun::bytes_t data;
        uint8_t* cursor;
        
        primitive_type_t last_primitive_type;
        uint8_t pixel_array_size;
    };
}
