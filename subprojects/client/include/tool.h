#pragma once

#include <FunEngine2D/core/include/color.h>

namespace space {

enum struct tool_mode_t {
    brush,
};

struct tool_t {
    tool_mode_t mode;
    fun::rgb_t color;
};

}