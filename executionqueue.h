#ifndef EXECUTIONQUEUE_H
#define EXECUTIONQUEUE_H

#include <QObject>
#include <QList>
#include <QSet>
#include <QDebug>

class Execution;

#include "execution.h"

// todo: customizable search order
// todo: remember the event sequence to reach each state

template <typename T>
class ExecutionQueue
{
protected:
    QSet<T> reachedState_;
    QList<Execution*> queue_;

public:
    explicit ExecutionQueue() {}

    bool contains(T state) {
        return reachedState_.contains(state);
    }

    void push(T state, Execution *execution) {
        reachedState_.insert(state);
        queue_.push_back(execution);
    }

    void unpop(Execution *execution) {
        queue_.push_front(execution);
    }

    Execution *pop() {
        if (queue_.empty()) {
            return nullptr;
        }
        return queue_.takeFirst();
    }

    int size() {
        return queue_.size();
    }

};

#endif // EXECUTIONQUEUE_H
