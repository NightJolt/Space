#pragma once

#include <FunEngine2D/core/include/rpc/rpc.h>

#include <common/include/packet.h>
#include <build/rpc/space/common.h>

namespace space::connection {
    struct connection_data_t {
        packet_t packet;
        fun::rpc::rpc_t rpc;
        std::unique_ptr<fun::rpc::stub_t<space::i_server_t>> server;
    };

    void init(connection_data_t&);
    void update(connection_data_t&);
    void exit(connection_data_t&);

    bool connect(connection_data_t&, fun::rpc::addr_t);
    void disconnect(connection_data_t&);

    void send_packet(connection_data_t&);

    bool is_connected(connection_data_t&);
}