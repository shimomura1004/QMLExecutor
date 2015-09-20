#include "helper.h"
#include <QQmlContext>
#include <QQuickWindow>
#include <QDir>
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
    }
}

void Helper::execute()
{
    currentExecution_->engine = new QQmlApplicationEngine(path_);
    currentExecution_->engine->rootContext()->setContextProperty("Helper", this);

    replay(currentExecution_);

    auto state = currentExecution_->engine->rootObjects().first()->property("state").toString();
    qDebug() << "state:" << state;
    if (!reachedState_.contains(state)) {
        reachedState_.insert(state);

        QSet<QObject*> objects;
        findClickableEventHandlers(currentExecution_, objects);

        foreach (auto obj, objects) {
            Execution *execution = new Execution;
            QString id = getId(currentExecution_->engine, obj);
            execution->eventSequence = currentExecution_->eventSequence;
            execution->eventSequence.push_back(id);
            queue_.push_back(execution);
        }

        static int count = 0;
        QString path = QDir::currentPath() + QStringLiteral("/hoge%1.jpg").arg(count);
        auto window = qobject_cast<QQuickWindow*>(currentExecution_->engine->rootObjects().first());
        window->grabWindow().save(path);
        count++;
    }
    else {
        qDebug() << "end";
    }

    delete currentExecution_->engine;
    delete currentExecution_;

    if (queue_.empty()) {
        return;
    }

    if (queue_.length() > 20) {
        qCritical() << "To many state";
        return;
    }

    currentExecution_ = queue_.takeFirst();
    execute();
}

void Helper::run()
{
    currentExecution_ = new Execution;
    execute();
}

