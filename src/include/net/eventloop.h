/*
EventLoop类，
*/

#ifndef RPC_NET_EVENTLOOP_H
#define RPC_NET_EVENTLOOP_H

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include "net/fd_event/fd_event.h"
#include "net/time/timer.h"
#include "net/wakeup_fd_event.h"
#include "time/time_event.h"

namespace rpc {
class EventLoop : public std::enable_shared_from_this<EventLoop> {
public:
    EventLoop();

    ~EventLoop();

    // EventLoop 启动！
    void loop();

    // 唤醒这个eventloop
    void wake_up();

    // 停止，但是一般不会停止，在服务器上会一直运行
    void stop();

    void add_epoll_event(FdEvent* event);

    void delete_epoll_event(FdEvent* event);

    bool is_in_loop_thread();

    void add_task(std::function<void()> task, bool is_wake_up = false);

    void add_timer_event(rpc::TimerEvent::s_ptr shard_ptr); // 添加定时任务
public:
    // 得到全局对象EventLoop
    static std::shared_ptr<EventLoop> get_current_eventloop();

private:
    void deal_wake_up();

    void add_to_epoll(FdEvent* event);

    void delete_from_epoll(FdEvent* event);

    void init_wakeup_fd_event();

    void init_timer();

private:
    pid_t m_thread_id { 0 };                    // 线程号
    int m_epoll_fd { 0 };                       // epoll句柄
    int m_wakeup_fd { 0 };                      // 唤醒标识符
    WakeUPEvent* m_wakeup_fd_event { nullptr }; // 唤醒的事件
    bool m_stop_flag { false };                 // eventloop是否暂停？
    std::set<int> m_listen_fds; // 监听的套接字，存入的是文件描述符
    std::queue<std::function<void()>> m_pending_tasks; // 待执行的任务队列。
    std::mutex m_mtx;                                  // 这个是锁
    Timer* m_timer { nullptr };
};
} // namespace rpc

#endif