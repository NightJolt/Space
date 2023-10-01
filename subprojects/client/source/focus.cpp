#include "focus.h"

#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/data/gridmap/defs.h>
#include <FunEngine2D/core/include/render/sprite.h>
#include <FunEngine2D/core/include/resources.h>

namespace {
    fun::render::sprite_t focus_rect;
}

void space::focus::init() {
    focus_rect.bind_texture(fun::resources::get_texture("focus"));
    focus_rect.set_origin({ 0, 0 });
}

void space::focus::update(state_t& state) {
    fun::render::window_t& window = fun::render::winmgr::get_main_window();

    auto grid_pos = fun::data::world_to_grid(window.get_mouse_world_position(), state.canvas.get_meshmap()->s_tile_size);

    focus_rect.set_position(grid_pos);
    focus_rect.set_color(state.canvas.get_texel(grid_pos).invert());

    window.draw_world(focus_rect, 1);
}