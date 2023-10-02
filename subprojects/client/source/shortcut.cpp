#include "shortcut.h"

#include <FunEngine2D/core/include/input.h>

namespace {
    std::unordered_map<sf::Keyboard::Key, space::shortcut::callback_t> callbacks;
    std::unordered_map<sf::Keyboard::Key, space::shortcut::callback_t> callbacks_shifted;
}

void space::shortcut::register_key(sf::Keyboard::Key key, callback_t callback, bool shifted) {
    if (shifted) {
        callbacks_shifted[key] = callback;
    } else {
        callbacks[key] = callback;
    }
}

void space::shortcut::invoke() {
    for (auto& [key, callback] : callbacks) {
        if (fun::input::pressed(key)) {
            callback();
        }
    }

    for (auto& [key, callback] : callbacks_shifted) {
        if (fun::input::pressed(key) && fun::input::hold(sf::Keyboard::LShift)) {
            callback();
        }
    }
}
