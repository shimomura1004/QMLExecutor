#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>
#include <QSet>

#include "execution.h"

// todo: support in-handler branching

class Executor : public QObject
{
    Q_OBJECT

protected:
    QList<Execution*> queue_;
    QSet<QString> reachedState_;

public:
    explicit Executor(QUrl path, QObject *parent = 0);
    void execute();

};

#endif // EXECUTOR_H
