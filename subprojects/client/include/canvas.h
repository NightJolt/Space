#pragma once

#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/render/meshmap/meshmap.h>
#include <FunEngine2D/core/include/color.h>

namespace space {

class canvas_t : public sf::Drawable {
private:
    typedef fun::render::meshmap_t<128, 1.f> canvas_mesh_t;

public:
    canvas_t();
    ~canvas_t() override = default;

    void set_texel(fun::vec2i_t, fun::rgb_t);
    fun::rgb_t get_texel(fun::vec2i_t);

    canvas_mesh_t* get_meshmap();

private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
    canvas_mesh_t meshmap;
};

}