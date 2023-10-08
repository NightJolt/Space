#include "shortcut.h"

#include <FunEngine2D/core/include/input.h>

namespace {
    struct key_data_t {
        sf::Keyboard::Key key;
        space::shortcut::key_flags_t flags;
    };

    std::unordered_map<
        key_data_t,
        space::shortcut::callback_t,
        decltype(
            [] (key_data_t data) {
                return fun::hash(fun::vec2i_t { int32_t { data.key }, data.flags });
            }
        ),
        decltype(
            [] (key_data_t a, key_data_t b) {
                return a.key == b.key && a.flags == b.flags;
            }
        )
    > callbacks;
}

void space::shortcut::register_key(sf::Keyboard::Key key, callback_t callback, key_flags_t flags) {
    callbacks[{ key, flags }] = callback;
}

void space::shortcut::invoke() {
    for (auto& [data, callback] : callbacks) {
        if (!fun::input::pressed(data.key)) continue;
        if ((data.flags & key_flags_t::control ? true : false) != fun::input::hold(sf::Keyboard::LControl)) continue;
        if ((data.flags & key_flags_t::shift ? true : false) != fun::input::hold(sf::Keyboard::LShift)) continue;
        if ((data.flags & key_flags_t::alt ? true : false) != fun::input::hold(sf::Keyboard::LAlt)) continue;

        callback();
    }
}
