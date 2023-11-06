#include "net/fd_event/fd_event.h"
#include <cstring>
#include <sys/epoll.h>
namespace rpc {
FdEvent::FdEvent(int fd) : m_fd(fd) {
    std::memset(&m_listen_events, 0, sizeof(m_listen_events));
}

FdEvent::FdEvent() {}

FdEvent::~FdEvent() { memset(&m_listen_events, 0, sizeof(m_listen_events)); }

int FdEvent::get_fd() const noexcept { return m_fd; }

epoll_event FdEvent::get_epoll_event() const noexcept { return m_listen_events; }

// 如果是读事件，那么就执行读回掉函数，否则，就是写回调函数
std::function<void()> FdEvent::handler(TriggerEvent event_type) {
    if (event_type == TriggerEvent::IN_EVENT) {
        return m_read_callback;
    }
    return m_write_callback;
}

void FdEvent::listen(TriggerEvent event_type, std::function<void()> callback) {
    if (event_type == TriggerEvent::IN_EVENT) {
        m_listen_events.events |= EPOLLIN;
        m_read_callback = callback;
    } else {
        m_listen_events.events |= EPOLLOUT;
        m_write_callback = callback;
    }
    m_listen_events.data.ptr = this;
}
} // namespace rpc