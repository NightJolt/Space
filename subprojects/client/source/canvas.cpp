#include "canvas.h"

#include <FunEngine2D/core/include/render/texture.h>

space::canvas_t::canvas_t() : meshmap(fun::render::texture_t {}, fun::rgb::black) {}

void space::canvas_t::set_texel(fun::vec2i_t grid_pos, fun::rgb_t color) {
    meshmap.set_color(grid_pos, color);
}

fun::rgb_t space::canvas_t::get_texel(fun::vec2i_t grid_pos) {
    return meshmap.get_color(grid_pos);
}

space::canvas_t::canvas_mesh_t* space::canvas_t::get_meshmap() {
    return &meshmap;
}

void space::canvas_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(meshmap, states);
}
