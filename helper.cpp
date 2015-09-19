#include "helper.h"
#include <QQmlContext>
#include <QDebug>
#include <QTimer>

#include <private/qquickmousearea_p.h>
#include <private/qquickevents_p_p.h>

Helper::Helper(QUrl path, QObject *parent)
    : QObject(parent)
    , path_(path)
{
}

QString Helper::getId(QQmlApplicationEngine *engine, QObject *obj)
{
    QQmlContext *context = engine->contextForObject(obj);

    if (!context) {
        return "";
    }
    return context->nameForObject(obj);
}

void Helper::findAvailableEventHandlers(Execution* execution, QSet<QObject*> &objects, bool(*judge)(QObject*))
{
    foreach (auto obj, execution->engine->rootObjects()) {
        traverse(obj, objects, judge);
    }
}

void Helper::findClickableEventHandlers(Execution* execution, QSet<QObject*> &objects)
{
    findAvailableEventHandlers(execution, objects, [](QObject *obj){
        bool visible = obj->property("visible").toBool();
        bool enabled = obj->property("enabled").toBool();
        bool hasClickedHandler = obj->metaObject()->indexOfMethod("clicked(QQuickMouseEvent*)") >= 0;
        return visible && enabled && hasClickedHandler;
    });
}

void Helper::replay(Helper::Execution *execution)
{
    qDebug() << "###########";
//    QEventLoop loop;
//    QTimer::singleShot(1000, &loop, SLOT(quit()));
//    loop.exec();

    EventSequence eventSequence = execution->eventSequence;
    foreach (auto id, eventSequence) {
        qDebug() << "clicking:" << id;

        QSet<QObject*> objects;
        traverse(execution->engine->rootObjects().first(), objects, [&](QObject *obj){
            return getId(execution->engine, obj) == id;
        });

        if (objects.size() != 1) {
            qWarning() << "bad identifier:" << id;
            return;
        }

        auto event = new QQuickMouseEvent(0, 0, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
        QMetaObject::invokeMethod(objects.toList().first(), "clicked", Q_ARG(QQuickMouseEvent*, event));

//        QEventLoop loop;
//        QTimer::singleShot(1000, &loop, SLOT(quit()));
//        loop.exec();
    }
}

void Helper::execute()
{
    currentExecution_->engine = new QQmlApplicationEngine(path_);
    currentExecution_->engine->rootContext()->setContextProperty("Helper", this);

    replay(currentExecution_);

    QSet<QObject*> objects;
    findClickableEventHandlers(currentExecution_, objects);

    foreach (auto obj, objects) {
        Execution *execution = new Execution;
        QString id = getId(currentExecution_->engine, obj);
        execution->eventSequence = currentExecution_->eventSequence;
        execution->eventSequence.push_back(id);
        queue_.push_back(execution);
    }

    delete currentExecution_->engine;
    delete currentExecution_;

    if (queue_.empty()) {
        return;
    }

    qDebug() << "length" << queue_.length();
    if (queue_.length() > 20)
        return;

    currentExecution_ = queue_.takeFirst();
    execute();
}

void Helper::run()
{
    currentExecution_ = new Execution;
    execute();
}

