#include <memory>
#include "fmt/core.h"
#include "net/rpc/rpc_channel.h"
#include "net/rpc/rpc_closure.h"
#include "net/rpc/rpc_controller.h"
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

// void test_tcp_dispatchor_client() {
//     std::shared_ptr<rpc::IPv4NetAddr> addr =
//         std::make_shared<rpc::IPv4NetAddr>("127.0.0.1", 12347);
//     std::shared_ptr<rpc::TcpClient> client = std::make_shared<rpc::TcpClient>(addr);

//     client->connect([addr, &client]() {
//         DEBUG_LOG(fmt::format("connect to {} success", addr->to_string()));
//         std::shared_ptr<rpc::ProtobufProtocol> message =
//             std::make_shared<rpc::ProtobufProtocol>();
//         message->m_msg_id = "114514";
//         message->m_pb_data = "test pb data";

//         makeOrderRequest request;
//         request.set_price(100);
//         request.set_goods("apple");

//         // 序列化
//         if (!request.SerializeToString(&(message->m_pb_data))) {
//             ERROR_LOG("serialize error");
//             return;
//         }

//         message->m_method_name = "Order.makeOrder";

//         client->write_message(message,
//                               [](std::shared_ptr<rpc::AbstractProtocol> msg_ptr) {
//                                   DEBUG_LOG("send mssage success");
//                               });
//         DEBUG_LOG("YES,下面是收到的信息");
//         client->read_message(
//             "114514", [](std::shared_ptr<rpc::AbstractProtocol> msg_ptr) {
//                 std::shared_ptr<rpc::ProtobufProtocol> message =
//                     std::dynamic_pointer_cast<rpc::ProtobufProtocol>(msg_ptr);
//                 DEBUG_LOG(fmt::format("req_id {}, get response {}", message->m_msg_id,
//                                       message->m_pb_data));

//                 makeOrderResponse response;
//                 if (!response.ParseFromString(message->m_pb_data)) {
//                     ERROR_LOG("寄了");
//                     return;
//                 }
//                 INFO_LOG(fmt::format("message success, response = {}",
//                                      response.ShortDebugString()));
//             });
//     });
// }

void test_rpc_channel_client() {
    std::shared_ptr<rpc::IPv4NetAddr> addr =
        std::make_shared<rpc::IPv4NetAddr>("127.0.0.1", 12347);
    std::shared_ptr<rpc::RpcChannel> channel = std::make_shared<rpc::RpcChannel>(addr);

    std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();
    request->set_price(100);
    request->set_goods("apple");

    std::shared_ptr<makeOrderResponse> response = std::make_shared<makeOrderResponse>();

    std::shared_ptr<rpc::RpcController> controller =
        std::make_shared<rpc::RpcController>();
    controller->set_msg_id("114514");

    std::shared_ptr<rpc::RpcClosure> closure =
        std::make_shared<rpc::RpcClosure>([request, response, channel]()  mutable {
            INFO_LOG(fmt::format("call rpc success, request = {}, response = {}",
                                 request->ShortDebugString(),
                                 response->ShortDebugString()));
            // 退出 loop循环
            INFO_LOG("exit eventloop")
            channel->get_client()->stop();
            channel.reset();
        });

    channel->init(controller, request, response, closure);
    Order_Stub stub(channel.get());

    stub.makeOrder(controller.get(), request.get(), response.get(), closure.get());
}

int main() {
    rpc::LogConfig::SET_GLOBAL_CONFIG("/home/lzc/code/tiny_rpc/conf/rpc.xml");

    rpc::Logger::INIT_GLOBAL_LOGGER();

    // test_tcp_dispatchor_client();
    test_rpc_channel_client();
}