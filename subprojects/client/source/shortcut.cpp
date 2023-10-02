#include "shortcut.h"

#include <FunEngine2D/core/include/input.h>

namespace {
    std::unordered_map<sf::Keyboard::Key, space::shortcut::callback_t> callbacks;
}

void space::shortcut::register_key(sf::Keyboard::Key key, callback_t callback) {
    callbacks[key] = callback;
}

void space::shortcut::invoke() {
    for (auto& [key, callback] : callbacks) {
        if (fun::input::pressed(key)) {
            callback();
        }
    }
}
