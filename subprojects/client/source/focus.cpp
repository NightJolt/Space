#include "focus.h"

#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/data/gridmap/defs.h>
#include <FunEngine2D/core/include/render/sprite.h>
#include <FunEngine2D/core/include/resources.h>

void space::focus::init(state_t& state) {
    state.focus_rect.bind_texture(fun::resources::get_texture("focus"));
    state.focus_rect.set_origin({ 0, 0 });
}

void space::focus::update(state_t& state) {
    fun::render::window_t& window = fun::render::winmgr::get_main_window();

    auto grid_pos = fun::data::world_to_grid(window.get_mouse_world_position(), state.canvas.s_tile_size);

    state.focus_rect.set_position(grid_pos);
    state.focus_rect.set_color(state.canvas.get_color(grid_pos).invert());

    window.draw_world(state.focus_rect, 10);
}