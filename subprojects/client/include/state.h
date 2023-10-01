#pragma once

#include "canvas.h"
#include "tool.h"

namespace space {

struct state_t {
    canvas_t canvas;
    tool_t tool;
};

}