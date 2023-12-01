#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

#define DEFAULT_CAPACITY 20

template <typename T>
class ThreadSafeQueue {
    typedef T type_name;

public:
    void push(type_name &&value) {
        std::unique_lock<std::mutex> lock(mutex_);
        isNotQueueFull_.wait(lock, [this]() {
            return queue_.size() < capacity_;
        });

        queue_.push(std::forward<type_name>(value));

        isNotQueueEmpty_.notify_all();
    }

    bool try_pop(type_name &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        // if (!isNotQueueEmpty_.wait_for(
        //         lock, std::chrono::milliseconds(100),
        //         [this]() { return !queue_.empty(); }
        //     )) {
        //     return false;
        // }

        isNotQueueEmpty_.wait(lock, [this]() { return !queue_.empty(); });

        value = std::move(queue_.front());

        queue_.pop();
        isNotQueueFull_.notify_all();

        return true;
    }

private:
    std::queue<type_name> queue_;
    std::mutex mutex_;
    std::condition_variable isNotQueueEmpty_;
    std::condition_variable isNotQueueFull_;
    const int capacity_ = DEFAULT_CAPACITY;
};

#endif  // THREAD_SAFE_QUEUE_H