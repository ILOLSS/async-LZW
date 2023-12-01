#ifndef ASYNC_TASK_POOL_H
#define ASYNC_TASK_POOL_H

#include <atomic>
#include <memory>
#include "async_task.hpp"
#include "thread_guard.hpp"
#include "thread_safe_queue.hpp"

#define DEFAULT_COUNT_OF_WORKERS 8

class AsyncTaskPool {
public:
    AsyncTaskPool();

    void submit(std::unique_ptr<BaseTask> task_ptr);

    ~AsyncTaskPool();

private:
    ThreadSafeQueue<std::unique_ptr<BaseTask>> tasks_;
    std::atomic<bool> is_stop_;
    const int threadCount_ = DEFAULT_COUNT_OF_WORKERS;
};

#endif  // ASYNC_TASK_POOL_H