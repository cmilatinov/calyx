#pragma once

namespace Calyx::Editor {

    class ThreadPool {
        using Job = Function<void()>;

    public:
        explicit ThreadPool(int numThreads = 1);
        ~ThreadPool();

        template<typename F, typename... Args>
        auto Enqueue(F&& func, Args&&... args) -> Future<typename std::invoke_result_t<F, Args...>> {
            using ReturnType = std::invoke_result_t<F, Args...>;
            auto task = CreateRef<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<F>(func), std::forward<Args>(args)...)
            );
            Future<ReturnType> result = task->get_future();
            {
                UniqueLock<Mutex> lock(m_mutex);
                m_jobs.emplace([task](){ (*task)(); });
            }
            m_condition.notify_one();
            return result;
        }

    private:
        void Init();
        void Destroy();

    private:
        bool m_stop = false;
        ConditionVariable m_condition;
        Mutex m_mutex;
        Queue<Job> m_jobs;
        List<Thread> m_threads;

        void ThreadLoop();

    };

}