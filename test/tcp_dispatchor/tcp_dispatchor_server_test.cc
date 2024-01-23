#include <memory>
#include "fmt/core.h"
#include "net/rpc/rpc_dispatchor.h"
#include "net/tcp/tcp_server.h"
#include "order.pb.h" // 导入的proto生成的文件
#include "google/protobuf/service.h"
#include "common/log.h"
#include "net/coder/protobuf_protocol.h"
#include "net/coder/abstract_protocol.h"
#include "net/tcp/ipv4_net_addr.h"
#include "net/tcp/tcp_client.h"
#include "net/tcp/tcp_connection.h"
#include "common/log_config.h"
#include "net/coder/protobuf_protocol.h"

class OrderImpl : public Order {
public:
    void make_order(google::protobuf::RpcController* controller,
                    const ::make_order_request* request, ::make_order_response* response,
                    ::google::protobuf::Closure* done) override {
        if (request->price() < 4) {
            response->set_ret_code(-1);
            response->set_res_info("买不起");
            return;
        } else {
            response->set_order_id("1145148109");
        }
    }
};

// void test_tcp_dispatchor() {
//     std::shared_ptr<rpc::IPv4NetAddr> addr =
//         std::make_shared<rpc::IPv4NetAddr>("127.0.0.1", 11451);
//     std::shared_ptr<rpc::TcpClient> client = std::make_shared<rpc::TcpClient>(addr);

//     client->connect([addr, &client]() {
//         DEBUG_LOG(fmt::format("connect to {} success", addr->to_string()));
//         std::shared_ptr<rpc::ProtobufProtocol> message =
//             std::make_shared<rpc::ProtobufProtocol>();
//         message->m_msg_id = "12345678";
//         message->m_pb_data = "test pb data";
//         client->write_message(message,
//                               [](std::shared_ptr<rpc::AbstractProtocol> msg_ptr) {
//                                   DEBUG_LOG("send mssage success");
//                               });
//     });

//     client->read_message("12345678", [](std::shared_ptr<rpc::AbstractProtocol> msg_ptr)
//     {
//         std::shared_ptr<rpc::ProtobufProtocol> message =
//             std::dynamic_pointer_cast<rpc::ProtobufProtocol>(msg_ptr);
//         DEBUG_LOG(fmt::format("req_id {}, get response {}", message->m_msg_id,
//                               message->m_pb_data));
//     });
// }

void test_tcp_server() {
    std::shared_ptr<rpc::IPv4NetAddr> addr =
        std::make_shared<rpc::IPv4NetAddr>("127.0.0.1", 12347);
    DEBUG_LOG(fmt::format("create addr = {}", addr->to_string()));

    std::shared_ptr<rpc::TcpServer> tcp_server = std::make_shared<rpc::TcpServer>(addr);
    tcp_server->start();
}
int main() {
    rpc::LogConfig::SET_GLOBAL_CONFIG("/home/lzc/tiny_rpc/conf/rpc.xml");

    rpc::Logger::INIT_GLOBAL_LOGGER();

    std::shared_ptr<OrderImpl> order_ptr = std::make_shared<OrderImpl>();
    rpc::RpcDispatcher::GET_RPC_DISPATCHER()->register_service(order_ptr);

    // test_tcp_dispatchor();

    test_tcp_server();
}