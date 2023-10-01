#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/render/window/window_manager.h>
#include <FunEngine2D/core/include/render/window/window_data.h>
#include <FunEngine2D/core/include/render/window/window.h>
#include <FunEngine2D/core/include/_time.h>
#include <FunEngine2D/core/include/input.h>

#include "include/space.h"

int main() {
    fun::render::winmgr::init(fun::render::window_data_t("space client"));
    auto& window = fun::render::winmgr::get_main_window();

    space::init();

    while (window.is_open()) {
        fun::time::recalculate();
        fun::render::winmgr::update();
        fun::input::listen(window.is_focused() && !ImGui::GetIO().WantCaptureKeyboard, window.is_focused() && !ImGui::GetIO().WantCaptureMouse);

        space::update();
        
        window.display(fun::rgb::black);
    }

    space::exit();

    fun::render::winmgr::close();

    return 0;
}