#include "connection.h"

void space::connection::init(connection_data_t& data) {
    space::register_rpc_interfaces(data.rpc);

    data.rpc.init(fun::rpc::port_t(8000));
}

void space::connection::update(connection_data_t& data) {
    data.rpc.step();
}

bool space::connection::connect(connection_data_t& data, fun::rpc::addr_t addr) {
    data.server.reset(data.rpc.get_remote_storage(addr).request_object<space::i_server_t>(0xDEADBEEF));

    return data.server.get() != nullptr;
}

void space::connection::disconnect(connection_data_t& data) {
    data.server.reset();
}

void space::connection::send_packet(connection_data_t& data) {
    data.packet.finalize();
    if (data.server) data.server->send_draw(data.packet.data);
    data.packet.reset();
}

bool space::connection::is_connected(connection_data_t& data) {
    return data.server.get() != nullptr && data.server->is_valid();
}
