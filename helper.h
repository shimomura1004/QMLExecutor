#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>
#include <QSet>

class Helper : public QObject
{
    Q_OBJECT

    typedef QList<QString> EventSequence;

    struct Execution {
        QQmlApplicationEngine *engine;
        EventSequence eventSequence;
    };

protected:
    QUrl path_;

    Execution *currentExecution_;
    QList<Execution*> queue_;

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

    //void traverse(QObject *obj, QSet<QObject*> &objects, bool(*judge)(QObject*));
    void findAvailableEventHandlers(Execution* execution, QSet<QObject*> &objects, bool(*judge)(QObject*));
    void findClickableEventHandlers(Execution* execution, QSet<QObject*> &objects);

    void replay(Execution *execution);

    void execute();

public:
    explicit Helper(QUrl path, QObject *parent = 0);
    void run();

};

#endif // HELPER_H
