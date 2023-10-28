#pragma once

#include <FunEngine2D/core/include/networking/client.h>

#include <common/include/packet.h>

namespace space::connection {
    struct connection_data_t {
        packet_t packet;
        fun::network::client_t client;
    };

    void send_packet(connection_data_t&);
}