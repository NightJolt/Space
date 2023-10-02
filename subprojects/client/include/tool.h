#pragma once

#include <FunEngine2D/core/include/color.h>

namespace space {

enum struct tool_mode_t {
    brush,
    eraser,
    eyedropper,
    bucket
};

struct tool_t {
    tool_mode_t mode = tool_mode_t::brush;
    fun::rgb_t color = fun::rgb::white;
};

}