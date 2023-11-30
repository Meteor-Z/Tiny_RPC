/*
编解码器
网络传输的时候将数据进行编解码，
编码：将回调函数编码成网络字节序发送到另一个端
解码：将字节序解码成回调函数给自己用

具体功能交给每一个子类实现
*/

#ifndef RPC_NET_TCP_ABSTRACT_CODER_H
#define RPC_NET_TCP_ABSTRACT_CODER_H

#include <vector>
#include <memory>
#include "net/tcp/tcp_buffer.h"
#include "net/tcp/abstract_protocol.h"

namespace rpc {
class AbstractCoder {
public:
    // 编码,将其写入到 buffer 里面
    virtual void encode(std::vector<std::shared_ptr<AbstractProtocol>>& messages,
                        std::shared_ptr<TcpBuffer> out_buffer) = 0;
    // 解码
    virtual void decode(std::vector<std::shared_ptr<AbstractProtocol>>& out_messages,
                        std::shared_ptr<TcpBuffer> buffer) = 0;
    virtual ~AbstractCoder() {
        
    }
};
} // namespace rpc

#endif