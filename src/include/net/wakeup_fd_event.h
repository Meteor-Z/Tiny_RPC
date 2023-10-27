#ifndef RPC_NET_WAKEUP_EVENT_H
#define RPC_NET_WAKEUP_EVENT_H

#include "net/fd_event/fd_event.h"

namespace rpc {
    class WakeUPEvent : public FdEvent {
    public:
        WakeUPEvent(int fd);
        ~WakeUPEvent();
        void wakeup();
    };
}
#endif