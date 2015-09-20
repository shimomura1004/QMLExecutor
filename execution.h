#ifndef EXECUTION_H
#define EXECUTION_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>

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

    bool executed_;

public:
    explicit Execution(EventSequence eventSequence, QObject *parent = 0);
    virtual ~Execution();
    static Execution *copy(const Execution *execution, ID id);

    void execute();

    template <typename T>
    T getState(T(*f)(QQmlApplicationEngine *)) {
        return f(engine_);
    }

    void takeScreenshot(QString path);
    QSet<ID> getInvokableEventHandlers();
};

#endif // EXECUTION_H
