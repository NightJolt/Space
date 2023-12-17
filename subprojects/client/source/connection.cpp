#include "connection.h"

void space::connection::send_packet(connection_data_t& connection_data) {
    connection_data.packet.finalize();    
    connection_data.client.send(connection_data.packet.data.data(), connection_data.packet.bytes_used());
    connection_data.packet.reset();
}