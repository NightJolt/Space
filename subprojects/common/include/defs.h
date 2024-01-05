#pragma once

#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/data/gridmap/defs.h>

namespace space::constants {
    constexpr uint32_t max_packet_size { 1024 };
    constexpr fun::data::chunk_size_t chunk_size { 128 };
    constexpr fun::data::chunk_volume_t chunk_size_2d { chunk_size * chunk_size };
}