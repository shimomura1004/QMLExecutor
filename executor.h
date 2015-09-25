#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QObject>
#include "executionqueue.h"
#include "execution.h"

// todo: support in-handler branching

class Executor : public QObject
{
    Q_OBJECT

protected:
    ExecutionQueue<QString> queue_;

public:
    explicit Executor(QUrl path, QObject *parent = 0);
    void execute();

};

#endif // EXECUTOR_H
