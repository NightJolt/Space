#include "painter.h"

void space::painter::draw_pixel(state_t& state, fun::data::grid_pos_t pos, fun::rgb_t color) {
    state.canvas.set_color(pos, color);
}

void space::painter::draw_line(state_t& state, fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color) {
    const bool steep = (std::fabs(end.y - start.y) > std::fabs(end.x - start.x));

    if (steep) {
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
    }

    if (start.x > end.x) {
        std::swap(start, end);
    }

    const float dx = end.x - start.x;
    const float dy = std::fabs(end.y - start.y);
    
    float error = dx * .5f;
    const int32_t ystep = (start.y < end.y) ? 1 : -1;

    int32_t x = start.x;
    int32_t y = start.y;

    const int32_t end_x = end.x;

    for (; x <= end_x; x++) {
        if (steep) {
            state.canvas.set_color({ y, x }, color);
        }  else {
            state.canvas.set_color({ x, y }, color);
        }
                
        error -= dy;

        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void space::painter::draw_bucket(state_t& state, fun::data::grid_pos_t pos, fun::rgb_t color, uint32_t max_iterations) {
    std::queue <fun::vec2i_t> queue;
    fun::unordered_map_vec2_t <uint32_t, bool> visited;

    fun::vec2i_t start = pos;
    fun::rgb_t start_color = state.canvas.get_color(start);

    queue.push(start);

    while (!queue.empty() && max_iterations--) {
        fun::vec2i_t pos = queue.front();
        queue.pop();

        if (visited[pos]) continue;
        visited[pos] = true;

        if (state.canvas.get_color(pos) != start_color) continue;

        state.canvas.set_color(pos, color);

        queue.push({ pos.x + 1, pos.y });
        queue.push({ pos.x - 1, pos.y });
        queue.push({ pos.x, pos.y + 1 });
        queue.push({ pos.x, pos.y - 1 });
    }
}

void space::painter::draw_rect(state_t& state, fun::data::grid_pos_t start, fun::data::grid_pos_t end, fun::rgb_t color, bool filled, bool preview) {
    if (start.x > end.x) {
        std::swap(start.x, end.x);
    }

    if (start.y > end.y) {
        std::swap(start.y, end.y);
    }

    auto draw_fn = [&state, start, end, color]<class T>(T& canvas) {
        if (state.tool.mode == space::tool_mode_t::rectangle) {
            for (auto x = start.x; x <= end.x; x++) {
                canvas.set_color({ x, start.y }, color);
                canvas.set_color({ x, end.y }, color);
            }

            for (auto y = start.y; y <= end.y; y++) {
                canvas.set_color({ start.x, y }, color);
                canvas.set_color({ end.x, y }, color);
            }
        } else {
            for (int32_t x = start.x; x <= end.x; x++) {
                for (int32_t y = start.y; y <= end.y; y++) {
                    canvas.set_color({ x, y }, color);
                }
            }
        }
    };

    if (preview) {
        draw_fn(state.preview_canvas);
    } else {
        draw_fn(state.canvas);
    }
}
