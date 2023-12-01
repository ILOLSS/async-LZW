#ifndef TASK_H
#define TASK_H

class BaseTask {
public:
    virtual void operator()() = 0;
    virtual ~BaseTask(){};
};

#endif  // TASK_H