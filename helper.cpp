#include "helper.h"
#include <QQmlContext>

QString Helper::getId(QQmlApplicationEngine *engine, QObject *obj)
{
    QQmlContext *context = engine->contextForObject(obj);

    if (!context) {
        return "";
    }

    return context->nameForObject(obj);
}

QObject *Helper::getObject(QQmlApplicationEngine *engine, QString id)
{
    auto judge = [&](QObject *obj){
        return getId(engine, obj) == id;
    };

    foreach (auto obj, engine->rootObjects()) {
        if (auto result = find(obj, judge)) {
            return result;
        }
    }

    return nullptr;
}
