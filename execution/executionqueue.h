#ifndef EXECUTIONQUEUE_H
#define EXECUTIONQUEUE_H

#include <QObject>
#include <QList>
#include <QSet>
#include <QDebug>

class ExecutionBase;

#include "executionbase.h"

// todo: customizable search order
// todo: remember the event sequence to reach each state

template <typename T>
class ExecutionQueue
{
protected:
    QSet<T> reachedState_;
    QList<ExecutionBase*> queue_;

public:
    explicit ExecutionQueue() {}

    bool contains(T state) {
        return reachedState_.contains(state);
    }

    bool push(T state, ExecutionBase *execution) {
        foreach (auto e, queue_) {
            if (*e == *execution) {
                qDebug() << "DUP!";
                return false;
            }
        }

        reachedState_.insert(state);
        queue_.push_back(execution);

        return true;
    }

    bool unpop(T state, ExecutionBase *execution) {
        int index = 0;
        foreach (auto e, queue_) {
            if (*e == *execution) {
                qDebug() << "DUP!";
                queue_.removeAt(index);
                break;
            }
            index++;
        }

        reachedState_.insert(state);
        queue_.push_front(execution);

        return true;
    }

    ExecutionBase *pop() {
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
