#include "interaction.h"

#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/input.h>
#include <FunEngine2D/core/include/_math.h>

#include <client/include/slave.h>
#include <client/include/painter.h>

void space::interaction::handle(state_t& state) {
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
                    slave::send_pixel(state, grid_pos, color);
                } else {
                    fun::vec2f_t start = state.data.mouse.last_grid_pos;
                    fun::vec2f_t end = grid_pos;

                    slave::send_line(state, start, end, color);
                }

                state.data.mouse.last_grid_pos = grid_pos;
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
                slave::send_bucket(state, grid_pos, state.tool.color);
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

                if (fun::input::released(sf::Mouse::Left)) {
                    space::slave::send_rect(state, start, end, color, state.tool.mode == rectangle_filled);
                } else {
                    space::painter::draw_rect(state, start, end, color, state.tool.mode == rectangle_filled, true);
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
