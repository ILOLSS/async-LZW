#ifndef ASYNC_TASK_H
#define ASYNC_TASK_H

#include <future>
#include "base_task.hpp"

template <typename T = void>
class AsyncTask : public BaseTask {
public:
    AsyncTask(AsyncTask &&) = default;
    AsyncTask &operator=(AsyncTask &&) = default;

    template <typename Function, typename... Args>
    AsyncTask(Function &&function, Args &&...args)
        : function_(std::bind(
              std::forward<Function>(function),
              std::forward<Args>(args)...
          )) {
    }

    std::future<T> get_future() {
        return promise_.get_future();
    }

    void operator()() override {
        promise_.set_value(run());
    }

    ~AsyncTask() override {
    }

protected:
    AsyncTask() = default;

    virtual T run() {
        return function_();
    }

private:
    std::promise<T> promise_;
    std::function<T()> function_;
};

template <>
class AsyncTask<void> : public BaseTask {
public:
    AsyncTask(AsyncTask &&) = default;
    AsyncTask &operator=(AsyncTask &&) = default;

    template <class Function, class... Args>
    AsyncTask(Function &&function, Args &&...args)
        : function_(std::bind(
              std::forward<Function>(function),
              std::forward<Args>(args)...
          )) {
    }

    std::future<void> get_future() {
        return promise_.get_future();
    }

    void operator()() override {
        run();
        promise_.set_value();
    }

    ~AsyncTask() override {
    }

protected:
    AsyncTask() = default;

    virtual void run() {
        function_();
    }

private:
    std::promise<void> promise_;
    std::function<void()> function_;
};

#endif  // ASYNC_TASK_H