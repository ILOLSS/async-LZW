#ifndef EXECTIME_H
#define EXECTIME_H

#include <chrono>
#include <functional>

template <typename T = void>
class ExecTime {
public:
    template <class Function, class... Args>
    ExecTime(Function &&function, Args &&...args)
        : function_(std::bind(
              std::forward<Function>(function),
              maybe_wrap(std::forward<Args>(args))...
          )) {
    }

    ExecTime(const ExecTime &) = delete;
    ExecTime &operator=(const ExecTime &) = delete;

    T operator()(const std::string &message) {
        auto start = std::chrono::high_resolution_clock::now();
        T result = function_();
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << message << " : " << duration.count() << " microseconds"
                  << std::endl;

        return result;
    }

private:
    std::function<T()> function_;

    template <class U>
    std::reference_wrapper<U> maybe_wrap(U &val) {
        return std::ref(val);
    }

    template <class U>
    U &&maybe_wrap(U &&val) {
        return std::forward<U>(val);
    }
};

template <>
class ExecTime<void> {
public:
    template <class Function, class... Args>
    ExecTime(Function &&function, Args &&...args)
        : function_(std::bind(
              std::forward<Function>(function),
              maybe_wrap(std::forward<Args>(args))...
          )) {
    }

    void operator()(const std::string &message) {
        auto start = std::chrono::high_resolution_clock::now();
        function_();
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << message << " : " << duration.count() << " microseconds"
                  << std::endl;
    }

private:
    std::function<void()> function_;

    template <class U>
    std::reference_wrapper<U> maybe_wrap(U &val) {
        return std::ref(val);
    }

    template <class U>
    U &&maybe_wrap(U &&val) {
        return std::forward<U>(val);
    }
};

#endif  // EXECTIME_H