/**
 * Copyright (c) 2019-2020 CHENHAO.
 *
 * This file is part of tinychain.
 *
 * tinychain is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef MVS_THREADPOOL_HPP
#define MVS_THREADPOOL_HPP

#include <memory>
#include <functional>
#include <thread>
#include <stdexcept>
#include <boost/asio.hpp> 
#include <contrib/compat.hpp> 

#ifdef ENABLE_DEBUG
#include <iostream>
#endif

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/resource.h>
    #include <sys/syscall.h>
    #include <sys/types.h>
    #ifndef PRIO_MAX
        #define PRIO_MAX 20
    #endif
    #define THREAD_PRIORITY_ABOVE_NORMAL (-2)
    #define THREAD_PRIORITY_NORMAL 0
    #define THREAD_PRIORITY_BELOW_NORMAL 2
    #define THREAD_PRIORITY_LOWEST PRIO_MAX
#endif

namespace tinychain {

enum class thread_priority
{
    high,
    normal,
    low,
    lowest
};

static int get_thread_priority(thread_priority priority)
{
    switch (priority)
    {
        case thread_priority::high:
            return THREAD_PRIORITY_ABOVE_NORMAL;
        case thread_priority::normal:
            return THREAD_PRIORITY_NORMAL;
        case thread_priority::low:
            return THREAD_PRIORITY_BELOW_NORMAL;
        case thread_priority::lowest:
            return THREAD_PRIORITY_LOWEST;
        default:
            throw std::invalid_argument("priority");
    }
}

static void set_thread_priority(thread_priority priority) {
    const auto prioritization = get_thread_priority(priority);
#if defined(_MSC_VER)
    SetThreadPriority(GetCurrentThread(), prioritization);
#elif defined(PRIO_THREAD)
    setpriority(PRIO_THREAD, pthread_self(), prioritization);
#else
    setpriority(PRIO_PROCESS, syscall(SYS_gettid), prioritization);
#endif
}

/**
 * A collection of threads which can be passed operations through io_service.
 */
class threadpool
{
public:
    using io_service = boost::asio::io_service;

    /**
     * Threadpool constructor. spawn()s number_threads threads.
     * @param[in]   number_threads  Number of threads to spawn.
     * @param[in]   priority        Priority of threads to spawn.
     */
     threadpool(size_t number_threads=0,
        thread_priority priority=thread_priority::normal) {
        service_ = std::make_shared<io_service>();
	    spawn(number_threads, priority);
	}

    ~threadpool() {}

    threadpool(const threadpool&) = delete;
    void operator=(const threadpool&) = delete;

    /**
     * Add n threads to this threadpool.
     * @param[in]   number_threads  Number of threads to add.
     * @param[in]   priority        Priority of threads to add.
     */
    void spawn(size_t number_threads=1,
        thread_priority priority=thread_priority::normal) {
    	for (size_t i = 0; i < number_threads; ++i)
    	    spawn_once(priority);
	}

    /**
     * Abandon outstanding operations without dispatching handlers.
     * Terminate threads once work is complete.
     * Prevents the enqueuing of new handlers.
     * Caller should next call join, which will block until complete.
     *
     * WARNING: This can result in leaks in the case of heap allocated objects
     * referenced captured in handlers that may not execute.
     */
    void abort() {
       service_->stop();
    }

    /**
     * Allow outstanding operations and handlers to finish normally.
     * Terminate threads once work is complete.
     * Allows the enqueuing of new handlers.
     * Caller should next call join, which will block until complete.
     */
    void shutdown() {
	    work_ = nullptr;
    }

    /**
     * Wait for all threads in the pool to terminate.
     *
     * WARNING: Do not call this within any of the threads owned by this
     * threadpool. Doing so will cause a resource deadlock and an
     * std::system_error exception will be thrown.
     */
    void join() {
        for (auto& thread: threads_)
            if (thread.joinable())
                thread.join();

        // This allows the pool to be cleanly restarted by calling spawn.
        threads_.clear();
        service_->reset();
    }

    /**
     * Underlying boost::io_service object.
     */
    std::shared_ptr<io_service> service() {
      return service_;
    }

private:
    void spawn_once(thread_priority priority=thread_priority::normal) {
    	// In C++14 work should use a unique_ptr.
    	// Work prevents the service from running out of work and terminating.
    	if (!work_)
    	    work_ = std::make_shared<io_service::work>(*service_);

    	const auto action = [this, priority]
    	{
    	    set_thread_priority(priority);
        #ifdef ENABLE_DEBUG
            std::cout<<"ENABLE_DEBUG new thread started pid: "<<syscall(SYS_gettid)<<std::flush<<std::endl;
        #endif
    	    service_->run();
    	};

    	threads_.push_back(std::thread(action));
	}

    std::shared_ptr<io_service> service_;
    std::vector<std::thread> threads_;
    std::shared_ptr<io_service::work> work_;
};

} // namespace libbitcoin


#endif
