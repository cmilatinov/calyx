#include "background/ThreadPool.h"

namespace Calyx::Editor {

    ThreadPool::ThreadPool(int numThreads)
        : m_threads(numThreads) {
        Init();
    }

    ThreadPool::~ThreadPool() {
        Destroy();
    }

    void ThreadPool::Init() {
        for (auto& thread: m_threads) {
            thread = Thread(&ThreadPool::ThreadLoop, this);
        }
    }

    void ThreadPool::Destroy() {
        {
            UniqueLock<Mutex> lock(m_mutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (auto& thread: m_threads) {
            thread.join();
        }
    }

    void ThreadPool::ThreadLoop() {
        while (true) {
            Job job;
            {
                UniqueLock<Mutex> lock(m_mutex);
                m_condition.wait(lock, [this]() {
                    return m_stop || !m_jobs.empty();
                });
                if (m_stop && m_jobs.empty()) {
                    return;
                }
                job = std::move(m_jobs.front());
                m_jobs.pop();
            }
            job();
        }
    }

}