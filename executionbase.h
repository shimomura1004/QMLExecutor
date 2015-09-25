#ifndef EXECUTIONBASE_H
#define EXECUTIONBASE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>

template <typename T> class ExecutionQueue;

#include "executionqueue.h"

class ExecutionBase : public QObject
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

    virtual ExecutionBase *copy(ID id) = 0;
    virtual QString getState() = 0;
    virtual QList<ID> getInvokableEventHandlers() = 0;
    virtual void takeScreenshot(QString path);

    Q_INVOKABLE virtual bool branch(bool condition);

public:
    explicit ExecutionBase(EventSequence eventSequence, QObject *parent = 0);
    virtual ~ExecutionBase();

    virtual bool execute(ExecutionQueue<QString> *queue);

};

#endif // EXECUTIONBASE_H
