#include "space.h"

#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/_time.h>
#include <FunEngine2D/core/include/input.h>
#include <FunEngine2D/core/include/resources.h>

#include "resources.h"
#include "state.h"
#include "focus.h"
#include "interface.h"
#include "shortcut.h"
#include "action.h"

namespace {
    space::state_t state;

    void setup_window() {
        fun::render::window_t& window = fun::render::winmgr::get_main_window();

        window.set_vsync(false);
        // window.target_framerate(144);
        window.set_world_view({ 0, 0 }, 64);
        window.set_cursor_visible(false);

        window.register_event_handler(sf::Event::MouseWheelMoved, [](fun::render::window_t& window, const sf::Event& event) {
            float zoom_value = event.mouseWheel.delta > 0 ? .9f : 1.1f;
            float zoom = window.get_zoom() * zoom_value;
            
            if (zoom < .1f || zoom > 30.f) return;
            
            window.zoom_into(window.get_mouse_screen_position(), zoom_value);
        });

        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    }

    void load_rplace() {
        fun::resources::load_texture("place", "../../resources/rplace2023.png");
        auto img = fun::resources::get_texture("place").get_texture()->copyToImage();

        for (int x = 0; x < img.getSize().x; x++) {
            for (int y = 0; y < img.getSize().y; y++) {
                state.canvas.set_texel({ x, y }, fun::rgb_t(img.getPixel(x, y)));
            }
        }
    }

    void setup_shortcuts() {
        space::shortcut::register_key(sf::Keyboard::B, []() {
            state.tool.mode = space::tool_mode_t::brush;
        });

        space::shortcut::register_key(sf::Keyboard::E, []() {
            state.tool.mode = space::tool_mode_t::eraser;
        });

        space::shortcut::register_key(sf::Keyboard::I, []() {
            state.tool.mode = space::tool_mode_t::eyedropper;
        });

        space::shortcut::register_key(sf::Keyboard::G, []() {
            state.tool.mode = space::tool_mode_t::bucket;
        });
    }
}

void space::init() {
    resources::load();

    setup_window();
    setup_shortcuts();
    // load_rplace();

    focus::init();
}

void space::update() {
    fun::render::window_t& window = fun::render::winmgr::get_main_window();
    window.move_world_view(fun::input::keyboard_2d() * window.get_zoom() * 50.f * fun::time::delta_time());

    focus::update(state);

    shortcut::invoke();
    action::handle(state);

    window.draw_world(state.canvas, 0);

    interface::draw(state);
}

void space::exit() {

}