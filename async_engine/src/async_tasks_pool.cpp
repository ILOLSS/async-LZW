#include "async_tasks_pool.hpp"
#include <iostream>

AsyncTaskPool::AsyncTaskPool() : is_stop_(false) {
    for (int i = 0; i < threadCount_; i++) {
        ThreadGuard([this]() {
            while (!is_stop_) {
                // std::unique_ptr<BaseTask> task_ptr(tasks_.pop());
                std::unique_ptr<BaseTask> task_ptr;
                if (!tasks_.try_pop(task_ptr)) {
                    continue;
                }
                task_ptr->operator()();
            }
        });
    }
}

void AsyncTaskPool::submit(std::unique_ptr<BaseTask> task_ptr) {
    tasks_.push(std::move(task_ptr));
}

AsyncTaskPool::~AsyncTaskPool() {
    is_stop_ = true;
}
