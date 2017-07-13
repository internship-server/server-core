//
// singleton_pool.hpp
//

#pragma once

#include <mutex>

#include "pool.hpp"
#include "null_mutex.hpp"

namespace core {

template <typename Tag,
    unsigned RequestedSize = sizeof(Tag),
    typename Mutex = null_mutex>
class SingletonPool {
public:
    static void* Malloc()
    {
        std::lock_guard<Mutex> g(mutex_);
        return pool_.Malloc();
    }

    static void Free(void* const chunk)
    {
        std::lock_guard<Mutex> g(mutex_);
        pool_.Free(chunk);
    }

    static bool PurgeMemory()
    {
        std::lock_guard<Mutex> g(mutex_);
        return pool_.PurgeMemory();
    }

private:
    struct pool : Pool
    {
        pool() : Pool(RequestedSize) {}
    };

    inline static pool& get_pool()
    {
        return pool_;
    }

    static pool pool_;
    static Mutex mutex_;

    SingletonPool() {}
};

template <typename Tag,
    unsigned RequestedSize = sizeof(Tag),
    typename Mutex = null_mutex>
typename SingletonPool<Tag, RequestedSize, Mutex>::pool
    SingletonPool<Tag, RequestedSize, Mutex>::pool_;

template <typename Tag,
    unsigned RequestedSize = sizeof(Tag),
    typename Mutex = null_mutex>
Mutex SingletonPool<Tag, RequestedSize, Mutex>::mutex_;

}
