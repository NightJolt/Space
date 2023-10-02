#pragma once

#include <FunEngine2D/core/include/globals.h>

namespace space::shortcut {
    typedef fun::fn_t<void()> callback_t;

    void register_key(sf::Keyboard::Key, callback_t, bool = false);
    void invoke();
}