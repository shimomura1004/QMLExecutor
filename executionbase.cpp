#include "executionbase.h"
#include "helper.h"

//#include <private/qquickmousearea_p.h>
#include <private/qquickevents_p_p.h>

#include <QQmlContext>
#include <QQuickWindow>
#include <QDir>
#include <QDebug>

QUrl ExecutionBase::url_;

ExecutionBase::ExecutionBase(QObject *parent)
    : QObject(parent)
    , engine_(nullptr)
    , executionQueue_(nullptr)
{
}

ExecutionBase::~ExecutionBase()
{
}

bool ExecutionBase::execute(ExecutionQueue<QString> *queue)
{
    engine_ = new QQmlApplicationEngine(url_, this);
    engine_->rootContext()->setContextProperty("Executor", this);

    executionQueue_ = queue;

    // replay event sequence
    while (!eventSequence_.empty()) {
        auto targetId = eventSequence_.takeFirst();
        consumedEventSequence_.push_back(targetId);

        currentBranching_ = branching_.takeFirst();

        if (auto obj = Helper::getObject(engine_, targetId)) {
            auto event = new QQuickMouseEvent(0, 0, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
            QMetaObject::invokeMethod(obj, "clicked", Q_ARG(QQuickMouseEvent*, event));
        }
        else {
            qCritical() << "Unknown ID:" << targetId;
        }

        consumedBranching_.push_back(currentBranching_);
    }

    auto state = getState();

    if (queue->contains(state)) {
        return false;
    }

    // todo: create a new class exteinding QQmlApplicationEngine
    //       to add getInvokableMethod and takeScreenshot

    // save screenshot
    QString path = QDir::homePath() + QStringLiteral("/%1.png").arg(state);
    qDebug() << "Taking screenshot for state:" << state << path;
    takeScreenshot(path);

    auto ids = getInvokableEventHandlers();
    if (ids.empty()) {
        return false;
    }

    // add to queue
    foreach (auto id, ids.mid(1)) {
        auto exec = clone();
        exec->copyFrom(this);
        exec->restart();
        exec->addEvent(id);
        queue->push(state, exec);
    }

    // push this execution back for reusing qmlengine
    addEvent(ids.first());
    queue->unpop(this);

    return true;
}

void ExecutionBase::restart()
{
    eventSequence_ = consumedEventSequence_ + eventSequence_;
    branching_ = consumedBranching_ + branching_;
}

void ExecutionBase::addEvent(ExecutionBase::ID event)
{
    eventSequence_.push_back(event);
    branching_.push_back(Branching());
}

void ExecutionBase::copyFrom(ExecutionBase *execution)
{
    if (!execution) {
        return;
    }

    eventSequence_ = execution->eventSequence_;
    consumedEventSequence_ = execution->consumedEventSequence_;
    branching_ = execution->branching_;
    consumedBranching_ = execution->consumedBranching_;
}

void ExecutionBase::takeScreenshot(QString path)
{
    auto window = qobject_cast<QQuickWindow*>(engine_->rootObjects().first());
    if (window) {
        window->grabWindow().save(path);
    }
    else {
        qCritical() << "root object isn't QQuickWindow, but" << engine_->rootObjects().first();
    }
}

bool ExecutionBase::branch(bool condition)
{
//    if (currentBranching_.empty()) {
//        auto newExecution = copyFrom(id);
//        queue_;

//        return true;
//    }

    qDebug() << Q_FUNC_INFO << "called branch";
    return condition;
}
