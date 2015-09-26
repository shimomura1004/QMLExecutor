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
        currentBranching_.clear();
        currentConsumedBranching_.clear();

        auto targetId = eventSequence_.takeFirst();
        consumedEventSequence_.push_back(targetId);

        currentBranching_ = branchingSequence_.takeFirst();

        if (auto obj = Helper::getObject(engine_, targetId)) {
            auto event = new QQuickMouseEvent(0, 0, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
            QMetaObject::invokeMethod(obj, "clicked", Q_ARG(QQuickMouseEvent*, event));
        }
        else {
            qCritical() << "Unknown ID:" << targetId;
        }

        consumedBranchingSequence_.push_back(currentConsumedBranching_);
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
    consumedEventSequence_.clear();
    branchingSequence_ = consumedBranchingSequence_ + branchingSequence_;
    consumedBranchingSequence_.clear();
}

void ExecutionBase::addEvent(ExecutionBase::ID event)
{
    eventSequence_.push_back(event);
    branchingSequence_.push_back(Branching());
}

void ExecutionBase::copyFrom(ExecutionBase *execution)
{
    if (!execution) {
        return;
    }

    eventSequence_ = execution->eventSequence_;
    consumedEventSequence_ = execution->consumedEventSequence_;
    branchingSequence_ = execution->branchingSequence_;
    consumedBranchingSequence_ = execution->consumedBranchingSequence_;
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

bool ExecutionBase::branch(bool)
{
    qDebug() << this;
    if (currentBranching_.empty()) {
        ExecutionBase* exec = clone();
        exec->copyFrom(this);

        auto newBranching = currentConsumedBranching_;
        newBranching.push_back(false);
        exec->branchingSequence_.push_front(newBranching);
        exec->restart();
        executionQueue_->push("", exec);

        qDebug() << "CLONE!" << currentConsumedBranching_;
        currentConsumedBranching_.push_back(true);
        return true;
    }

    bool b = currentBranching_.takeFirst();
    currentConsumedBranching_.push_back(b);

    qDebug() << "replay!" << currentConsumedBranching_;

    return b;
}
