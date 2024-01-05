#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/rpc/rpc.h>
#include <FunEngine2D/core/include/data/gridmap/gridmap.h>

#include <build/rpc/space/common.h>

#include <common/include/defs.h>
#include <common/include/packet.h>

fun::rpc::rpc_t rpc;
fun::data::gridmap_t<space::constants::chunk_size, fun::rgb_t> gridmap;

struct server_t : public space::i_server_t {
    server_t() {}

    void send_draw(const fun::bytes_t& data) override {
        PRINTLN(data.get_size());
    }

    fun::bytes_t get_chunk(int32_t x, int32_t y) override {
        space::packet_t packet;

        packet.finalize();

        return packet.get_data().clone();
    }
};

int main() {
    space::register_rpc_interfaces(rpc);

    rpc.init(8001);

    auto* server = rpc.create_object<server_t>();
    rpc.get_local_storage().store_object(0xDEADBEEF, server);

    rpc.run();

    return 0;
}