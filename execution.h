#ifndef EXECUTION_H
#define EXECUTION_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>

template <typename T> class ExecutionQueue;

#include "executionqueue.h"

class Execution : public QObject
{
    Q_OBJECT

public:
    // todo: use xpath instead of id
    typedef QString ID;

    // todo: event sequence should have target and event kind
    typedef QList<ID> EventSequence;

protected:
    static QUrl url_;

public:
    static void setRootQMLPath(QUrl url) {
        url_ = url;
    }

protected:
    QQmlApplicationEngine *engine_;
    EventSequence eventSequence_;
    EventSequence consumedEventSequence_;

    QString getState() {
        auto state = engine_->rootObjects().first()->property("state").toString();
        auto substate = engine_->rootObjects().first()->property("substate").toBool();
        return state + (substate ? "true" : "false");
    }

public:
    explicit Execution(EventSequence eventSequence, QObject *parent = 0);
    virtual ~Execution();
    static Execution *copy(const Execution *execution, ID id);

    bool execute(ExecutionQueue<QString> *queue);

//    template <typename T>
//    T getState(T(*f)(QQmlApplicationEngine *)) {
//        return f(engine_);
//    }

    void takeScreenshot(QString path);
    QList<ID> getInvokableEventHandlers();

    Q_INVOKABLE bool branch(bool condition);
};

#endif // EXECUTION_H
