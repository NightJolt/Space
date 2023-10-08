#pragma once

#include <FunEngine2D/core/include/globals.h>

namespace space::shortcut {
    typedef fun::fn_t<void()> callback_t;

    enum key_flags_t : int32_t {
        none = 0,
        control = 1,
        shift = 2,
        alt = 4
    };

    void register_key(sf::Keyboard::Key, callback_t, key_flags_t = key_flags_t::none);
    void invoke();
}