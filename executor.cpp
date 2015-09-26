#include "executor.h"
#include <QDebug>

Executor::Executor(QUrl path, QObject *parent)
    : QObject(parent)
{
    ExecutionBase::setRootQMLPath(path);
    queue_.push("", new Execution);
}

void Executor::execute()
{
    uint counter = 0;

    while(auto execution = queue_.pop()) {
        bool reused = execution->execute(&queue_);
        if (!reused) {
            delete execution;
        }

        if (counter++ > 30) {
            qCritical() << "Too much iteration(" << counter << "). Abort.";
            return;
        }
    }

    qDebug() << "Executed" << counter << "paths";
}
