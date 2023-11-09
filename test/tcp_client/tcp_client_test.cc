#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common/log_config.h"
#include "common/log.h"
#include "net/tcp/ipv4_net_addr.h"
#include "net/tcp/tcp_server.h"

void test_tcp_client() {
    // 1. 调用 connect 连接 server
    // 2. write 一个字符串
    // 3. 等待 read 返回结果

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        exit(0);
    }

    sockaddr_in server_addr {};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1245);
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    int rt = connect(fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

    std::string message = "hello world!";

    rt = write(fd, message.c_str(), message.size());
    
    rpc::utils::DEBUG_LOG(fmt::format("succes write {} bytes, {}", rt, message.c_str()));

    char buf[100];
    
    rt = read(fd, buf, 100);
    
    rpc::utils::DEBUG_LOG(fmt::format("read succes {} bytes, {}", rt, std::string(buf)));
    
    
    
}

int main() {
    rpc::LogConfig::SET_GLOBAL_CONFIG("/home/lzc/tiny_rpc/conf/rpc.xml");
    rpc::Logger::INIT_GLOBAL_LOGGER();

    test_tcp_client();
}