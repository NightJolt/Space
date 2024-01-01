#include <FunEngine2D/core/include/globals.h>
#include <FunEngine2D/core/include/rpc/rpc.h>

#include <build/rpc/space/common.h>

fun::rpc::rpc_t rpc;

struct server_t : public space::i_server_t {
    server_t() {}

    void send_draw(const fun::bytes_t& data) override {
        PRINTLN(data.get_size());
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