#include "action.h"

#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/input.h>
#include <FunEngine2D/core/include/_math.h>

void space::action::handle(state_t& state) {
    fun::render::window_t& window = fun::render::winmgr::get_main_window();

    using enum space::tool_mode_t;

    bool mouse_active = false;
    auto world_pos = window.get_mouse_world_position();
    auto grid_pos = fun::data::world_to_grid(world_pos, state.canvas.s_tile_size);

    switch (state.tool.mode) {
        case eraser:
        case brush: {
            if (fun::input::hold(sf::Mouse::Left)) {
                auto color = state.tool.mode == brush ? state.tool.color : fun::rgb::black;
                
                if (!state.data.mouse.last_frame_active) {
                    state.canvas.set_color(grid_pos, color);
                } else {
                    fun::vec2f_t start = fun::data::world_to_grid(state.data.mouse.last_world_pos, state.canvas.s_tile_size);
                    fun::vec2f_t end = grid_pos;
                    
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

                    state.data.mouse.last_grid_pos = { x, y };
                }

                state.data.mouse.last_world_pos = world_pos;
                mouse_active = true;
            }

            break;
        }

        case eyedropper: {
            if (fun::input::pressed(sf::Mouse::Left)) {
                state.tool.color = state.canvas.get_color(grid_pos);
                state.tool.mode = brush;
            }

            break;
        }

        case bucket: {
            if (fun::input::pressed(sf::Mouse::Left)) {
                std::queue <fun::vec2i_t> queue;
                fun::unordered_map_vec2_t <uint32_t, bool> visited;

                fun::vec2i_t start = grid_pos;
                fun::rgb_t start_color = state.canvas.get_color(start);

                fun::rgb_t color = state.tool.color;

                queue.push(start);

                uint32_t max_iterations = 10000;

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

            break;
        }

        case rectangle:
        case rectangle_filled: {
            if (fun::input::pressed(sf::Mouse::Left)) {
                state.data.mouse.last_grid_pos = grid_pos;
            } else if (fun::input::hold(sf::Mouse::Left) || fun::input::released(sf::Mouse::Left)) {
                auto color = fun::rgba_t { state.tool.color, 150 };

                auto start = state.data.mouse.last_grid_pos;
                auto end = grid_pos;

                if (fun::input::hold(sf::Keyboard::LShift)) {
                    auto diff = end - start;
                    auto signs = fun::math::signs(diff);
                    auto size = fun::math::abs(diff);
                    auto min = std::min(size.x, size.y);

                    end = start + fun::vec2i_t(min, min) * signs;
                }

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

                if (fun::input::released(sf::Mouse::Left)) {
                    draw_fn(state.canvas);
                } else {
                    draw_fn(state.preview_canvas);
                }
            }

            break;
        }

        case rectangular_selection: {
            if (fun::input::pressed(sf::Mouse::Left)) {
                state.selection.start = grid_pos;
                state.selection.end = grid_pos;
                state.selection.selecting = true;
                state.selection.selected = false;
            } else if (fun::input::hold(sf::Mouse::Left)) {
                state.selection.end = grid_pos;
            } else if (fun::input::released(sf::Mouse::Left)) {
                state.selection.selecting = false;
                state.selection.selected = true;
            }

            break;
        }

        default: {
            break;
        }
    }

    state.data.mouse.last_frame_active = mouse_active;

    if (state.selection.selecting || state.selection.selected) {
        auto color = fun::rgba_t { fun::rgb::white, 150 };

        auto start = state.selection.start;
        auto end = state.selection.end;

        if (start.x > end.x) {
            std::swap(start.x, end.x);
        }

        if (start.y > end.y) {
            std::swap(start.y, end.y);
        }

        for (auto x = start.x; x <= end.x; x++) {
            state.preview_canvas.set_color({ x, start.y }, color);
            state.preview_canvas.set_color({ x, end.y }, color);
        }

        for (auto y = start.y; y <= end.y; y++) {
            state.preview_canvas.set_color({ start.x, y }, color);
            state.preview_canvas.set_color({ end.x, y }, color);
        }
    }
}