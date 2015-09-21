#include "execution.h"
#include "helper.h"
#include "finders/clickableobjectfinder.h"

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
    if (window) {
        window->grabWindow().save(path);
    }
    else {
        qCritical() << "root object isn't QQuickWindow, but" << engine_->rootObjects().first();
    }
}

QSet<Execution::ID> Execution::getInvokableEventHandlers()
{
    QSet<Execution::ID> ids;

    foreach (auto object, engine_->rootObjects()) {
        foreach (auto item, ClickableObjectFinder::find(qobject_cast<QQuickWindow*>(object))) {
            ids.insert(Helper::getId(engine_, item));
        }
    }

    return ids;
}
