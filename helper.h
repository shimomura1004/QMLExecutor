#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QSet>
#include <QQmlApplicationEngine>

class Helper
{
private:
    Helper() {}

public:
    template <typename F>
    static QObject *find(QObject *obj, F &judge) {
        if (!obj) {
            return nullptr;
        }

        if (judge(obj)) {
            return obj;
        }

        foreach (auto child, obj->children()) {
            if (auto obj = find(child, judge)) {
                return obj;
            }
        }

        return nullptr;
    }

    template <typename F>
    static QSet<QObject*> findAll(QObject *obj, F &judge) {
        QSet<QObject*> objects;
        if (!obj) {
            return QSet<QObject*>();
        }

        if (judge(obj)) {
            objects.insert(obj);
        }

        foreach (auto child, obj->children()) {
            objects.unite(findAll(child, judge));
        }

        return objects;
    }

    static QString getId(QQmlApplicationEngine *engine, QObject *obj);
    static QObject *getObject(QQmlApplicationEngine *engine, QString id);

};

#endif // HELPER_H
