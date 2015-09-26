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
    typedef QList<bool> Branching;

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
    ExecutionQueue<QString> *executionQueue_;

    EventSequence eventSequence_;
    EventSequence consumedEventSequence_;

    QList<Branching> branching_;
    QList< QList<bool> > consumedBranching_;
    Branching currentBranching_;

    void addEvent(ID event);

    virtual ExecutionBase *clone() = 0;
    virtual void copyFrom(ExecutionBase *execution);
    virtual void restart();

    virtual QString getState() = 0;
    virtual QList<ID> getInvokableEventHandlers() = 0;
    virtual void takeScreenshot(QString path);

    Q_INVOKABLE virtual bool branch(bool condition);

public:
    explicit ExecutionBase(QObject *parent = 0);
    virtual ~ExecutionBase();

    virtual bool execute(ExecutionQueue<QString> *queue);

};

#endif // EXECUTIONBASE_H
