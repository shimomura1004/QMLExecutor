#include "executor.h"
#include <QDebug>

Executor::Executor(QUrl path, QObject *parent)
    : QObject(parent)
{
    Execution::setRootQMLPath(path);
    queue_.push("", new Execution(Execution::EventSequence()));
}

void Executor::execute()
{
    uint counter = 0;

    while(auto execution = queue_.pop()) {
        bool reused = execution->execute(&queue_);
        if (!reused) {
            delete execution;
        }

        if (counter++ > 20) {
            qCritical() << "Too much iteration. Abort.";
            return;
        }
    }

    qDebug() << "Executed" << counter << "paths";
}
