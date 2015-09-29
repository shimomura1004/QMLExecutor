#include "execution.h"
#include "finders/clickableobjectfinder.h"
#include "helper.h"

Execution *Execution::clone()
{
    return new Execution;
}

QString Execution::getState()
{
    auto state = engine_->rootObjects().first()->property("state").toString();
//    auto substate = engine_->rootObjects().first()->property("substate").toBool();
//    auto opacity = engine_->rootObjects().first()->property("opac").toDouble();
//    return state + (substate ? "true" : "false") + QString::number(opacity);
    return state;
}

Execution::Execution(QObject *parent)
    : ExecutionBase(parent)
{
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

