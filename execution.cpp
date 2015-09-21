#include "execution.h"
#include "helper.h"

#include <private/qquickmousearea_p.h>
#include <private/qquickevents_p_p.h>

#include <QQuickWindow>
#include <QDebug>

QUrl Execution::url_;

Execution::Execution(EventSequence eventSequence, QObject *parent)
    : QObject(parent)
    , engine_(nullptr)
    , eventSequence_(eventSequence)
    , executed_(false)
{
}

Execution::~Execution()
{
    delete engine_;
}

Execution *Execution::copy(const Execution *execution, Execution::ID id)
{
    EventSequence seq = execution->eventSequence_;
    seq.append(id);
    return new Execution(seq);
}

void Execution::execute()
{
    engine_ = new QQmlApplicationEngine(url_);

    foreach (auto targetId, eventSequence_) {
        if (auto obj = Helper::getObject(engine_, targetId)) {
            auto event = new QQuickMouseEvent(0, 0, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
            QMetaObject::invokeMethod(obj, "clicked", Q_ARG(QQuickMouseEvent*, event));
        }
        else {
            qCritical() << "Unknown ID:" << targetId;
        }
    }

    executed_ = true;
}

void Execution::takeScreenshot(QString path)
{
    auto window = qobject_cast<QQuickWindow*>(engine_->rootObjects().first());
    window->grabWindow().save(path);
}

// todo: consider parent property
QSet<Execution::ID> Execution::getInvokableEventHandlers()
{
    auto judge = [](QObject *obj){
        bool visible = obj->property("visible").toBool();
        bool enabled = obj->property("enabled").toBool();
        bool hasClickedHandler = obj->metaObject()->indexOfMethod("clicked(QQuickMouseEvent*)") >= 0;
        return visible && enabled && hasClickedHandler;
    };

    decltype(getInvokableEventHandlers()) ids;
    foreach (auto object, engine_->rootObjects()) {
        foreach (auto obj, Helper::findAll(object, judge)) {
            ids.insert(Helper::getId(engine_, obj));
        }
    }
    return ids;
}
