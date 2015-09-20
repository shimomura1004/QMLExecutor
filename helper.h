#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>
#include <QSet>

// todo: support in-handler branching

// todo: divide into small classes
class Helper : public QObject
{
    Q_OBJECT

    // todo: event sequence should have target and event kind
    typedef QList<QString> EventSequence;

    // todo: separate as class
    struct Execution {
        QQmlApplicationEngine *engine;
        EventSequence eventSequence;
    };

protected:
    QUrl path_;

    Execution *currentExecution_;
    QList<Execution*> queue_;
    QSet<QString> reachedState_;

    // todo: use xpath instead of id
    QString getId(QQmlApplicationEngine *engine, QObject *obj);

    template<typename Func>
    void traverse(QObject *obj, QSet<QObject*> &objects, Func judge) {
        if (!obj) {
            return;
        }

        if (judge(obj)) {
            objects.insert(obj);
        }

        foreach (auto child, obj->children()) {
            traverse(child, objects, judge);
        }
    }

    // todo: find handlers, excluding hiden ones
    void findAvailableEventHandlers(Execution* execution, QSet<QObject*> &objects, bool(*judge)(QObject*));
    void findClickableEventHandlers(Execution* execution, QSet<QObject*> &objects);

    // todo: optimize replay
    void replay(Execution *execution);

    void execute();

public:
    explicit Helper(QUrl path, QObject *parent = 0);
    void run();

};

#endif // HELPER_H
