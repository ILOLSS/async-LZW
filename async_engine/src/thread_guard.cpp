#include "thread_guard.hpp"

ThreadGuard::ThreadGuard(ThreadGuard &&other) {
    thread_ = std::move(other.thread_);
    is_joinable_ = true;
    other.is_joinable_ = false;
}

ThreadGuard &ThreadGuard::operator=(ThreadGuard &&other) {
    thread_ = std::move(other.thread_);
    is_joinable_ = true;
    other.is_joinable_ = false;
    return *this;
}

ThreadGuard::~ThreadGuard() {
    if (is_joinable_ == true) {
        thread_.detach();
    }
}
