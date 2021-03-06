//
// session_manager.hpp
//
// session in udp can be defined with client's endpoint
//

#pragma once

#pragma comment(lib, "lock.lib")

#include <map>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>

#include "Spinlock.h"
#include "memory_pool.hpp"

#include "session.hpp"
#include "socket.hpp"
#include "endpoint.hpp"

namespace core::udp {
class SessionManager {
public:
    SessionManager(Endpoint& endpoint, int num_thread);
    ~SessionManager()
    {
        have_to_stop_ = true;
        for (int i = 0; i < threads_.size(); i++) {
            threads_[i].join();
        }
    }

    bool Run();

    inline void SetAcceptHandler(std::function<void(Session*)> &ahandler)
    {
        is_inited_all |= 0x01;
        ahandler_ = ahandler;
    }

    inline void SetDisconnectHandler(std::function<void(Session*)> &dhandler)
    {
        is_inited_all |= 0x10;
        dhandler_ = dhandler;
    }

    inline void SetPacketHandler(std::function<void(Session*, Packet&)>
        &phandler)
    {
        is_inited_all |= 0x100;
        phandler_ = phandler;
    }

private:
    static const long wait_time = 3000;

    bool have_to_stop_;
    HANDLE iocp_;
    void iocp_task();
    
    std::unique_ptr<Socket> socket_;
    
    std::vector<std::thread> threads_;
    int num_thread_;

    unsigned short is_inited_all;

    std::function<void(Session*)> ahandler_;
    std::function<void(Session*)> dhandler_;
    std::function<void(Session*, Packet&)> phandler_;

    std::map<Endpoint, Session*> sessions_;
    std::map<Endpoint, Session*> pending_sessions_;

    core::Spinlock lock_sessions_;
    core::Spinlock lock_pending_sessions_;

    std::mutex mtx_sessions_;
    std::mutex mtx_pending_sessions_;

    static core::ObjectPool<Session> session_pool_;
};
}