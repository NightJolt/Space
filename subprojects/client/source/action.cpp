#include "action.h"

#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/input.h>

void space::action::handle(state_t& state) {
    fun::render::window_t& window = fun::render::winmgr::get_main_window();

    using enum space::tool_mode_t;

    bool mouse_active = false;

    if (fun::input::hold(sf::Mouse::Left)) {
        auto world_pos = window.get_mouse_world_position();
        auto grid_pos = fun::data::world_to_grid(world_pos, state.canvas.get_meshmap()->s_tile_size);

        switch (state.tool.mode) {
            case eraser:
            case brush: {
                auto color = state.tool.mode == brush ? state.tool.color : fun::rgb::black;
                
                if (!state.data.mouse.last_frame_active) {
                    state.canvas.set_texel(grid_pos, color);
                } else {
                    fun::vec2f_t start = fun::data::world_to_grid(state.data.mouse.last_world_pos, state.canvas.get_meshmap()->s_tile_size);
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
                            state.canvas.set_texel({ y, x }, color);
                        }  else {
                            state.canvas.set_texel({ x, y }, color);
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

                break;
            }

            case eyedropper: {
                state.tool.color = state.canvas.get_texel(grid_pos);
                state.tool.mode = brush;

                break;
            }

            case bucket: {
                std::queue <fun::vec2i_t> queue;
                fun::unordered_map_vec2_t <uint32_t, bool> visited;

                fun::vec2i_t start = grid_pos;
                fun::rgb_t start_color = state.canvas.get_texel(start);

                fun::rgb_t color = state.tool.color;

                queue.push(start);

                uint32_t max_iterations = 10000;

                while (!queue.empty() && max_iterations--) {
                    fun::vec2i_t pos = queue.front();
                    queue.pop();

                    if (visited[pos]) continue;
                    visited[pos] = true;

                    if (state.canvas.get_texel(pos) != start_color) continue;

                    state.canvas.set_texel(pos, color);

                    queue.push({ pos.x + 1, pos.y });
                    queue.push({ pos.x - 1, pos.y });
                    queue.push({ pos.x, pos.y + 1 });
                    queue.push({ pos.x, pos.y - 1 });
                }

                break;
            }

            default: {
                break;
            }
        }
    }

    state.data.mouse.last_frame_active = mouse_active;
}