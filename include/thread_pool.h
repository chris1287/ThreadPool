#ifndef CRP_THREAD_POOL_H
#define CRP_THREAD_POOL_H

#include <thread>
#include <vector>
#include <stack>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace crp {

class ThreadPool {
public:
    ThreadPool(unsigned int size = std::thread::hardware_concurrency())
        : m_size(size)
        , m_stop(false) {
        for(unsigned int i = 0; i < m_size; ++i) {
            m_pool.push_back(std::thread(std::bind(&ThreadPool::mainloop, this)));
        }
    }

    ThreadPool(ThreadPool const &) = delete;

    ThreadPool& operator=(ThreadPool const &) = delete;

    ~ThreadPool() {
        stop();
    }

    void add_task(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(m_pool_mutex);
            m_tasks.push(task);
        }
        m_pool_condition.notify_one();
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(m_pool_mutex);
            m_stop = true;
        }
        m_pool_condition.notify_all();
        for(auto &t : m_pool) {
            t.join();
        }
        m_pool.clear();
    }

private:
    void mainloop() {
        while(true) {
            std::function<void()> t;
            {
                std::unique_lock<std::mutex> lock(m_pool_mutex);
                m_pool_condition.wait(lock, [this](){
                    return !m_tasks.empty() || m_stop;
                });
                if(m_stop) {
                    break;
                }
                t = m_tasks.top();
                m_tasks.pop();
            }
            t();
        }
    }

private:
    unsigned int const m_size;
    bool m_stop;
    std::mutex m_pool_mutex;
    std::condition_variable m_pool_condition;
    std::vector<std::thread> m_pool;
    std::stack<std::function<void()>> m_tasks;
};

}

#endif // CRP_THREAD_POOL_H
