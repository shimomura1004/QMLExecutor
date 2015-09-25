#include "execution.h"
#include "finders/clickableobjectfinder.h"
#include "helper.h"

QString Execution::getState()
{
    auto state = engine_->rootObjects().first()->property("state").toString();
    auto substate = engine_->rootObjects().first()->property("substate").toBool();
    return state + (substate ? "true" : "false");
}

Execution::Execution(EventSequence eventSequence, QObject *parent)
    : ExecutionBase(eventSequence, parent)
{
}

Execution *Execution::copy(ExecutionBase::ID id)
{
    EventSequence seq = consumedEventSequence_;
    seq.append(id);
    return new Execution(seq);
}

QList<ExecutionBase::ID> Execution::getInvokableEventHandlers()
{
    QList<ExecutionBase::ID> ids;

    foreach (auto object, engine_->rootObjects()) {
        foreach (auto item, ClickableObjectFinder::find(qobject_cast<QQuickWindow*>(object))) {
            ids.push_back(Helper::getId(engine_, item));
        }
    }

    return ids;
}
