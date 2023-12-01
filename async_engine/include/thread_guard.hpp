#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H

#include <thread>

class ThreadGuard {
public:
    template <typename Function, typename... Args>
    ThreadGuard(Function &&f, Args &&...args)
        : thread_(std::forward<Function>(f), std::forward<Args>(args)...),
          is_joinable_(true) {
    }

    ThreadGuard(const ThreadGuard &) = delete;
    ThreadGuard &operator=(const ThreadGuard &) = delete;

    ThreadGuard(ThreadGuard &&);
    ThreadGuard &operator=(ThreadGuard &&);

    ~ThreadGuard();

private:
    std::thread thread_;
    bool is_joinable_;
};

#endif  // THREAD_GUARD_H
