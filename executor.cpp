#include "executor.h"
#include <QQmlContext>
#include <QQuickWindow>
#include <QDir>

#include <QDebug>
#include <QTimer>

#include <private/qquickmousearea_p.h>
#include <private/qquickevents_p_p.h>

Executor::Executor(QUrl path, QObject *parent)
    : QObject(parent)
{
    Execution::setRootQMLPath(path);
    queue_.push_back(new Execution(Execution::EventSequence()));
}

void Executor::execute()
{
    uint counter = 0;
    while (!queue_.isEmpty()) {
        Execution *execution = queue_.takeFirst();
        execution->execute();

        auto f = [](QQmlApplicationEngine *engine){
            auto state = engine->rootObjects().first()->property("state").toString();
            auto substate = engine->rootObjects().first()->property("substate").toBool();
            return state + (substate ? "true" : "false");
        };
        auto state = execution->getState<QString>(f);

        if (!reachedState_.contains(state)) {
            reachedState_.insert(state);

            auto ids = execution->getInvokableEventHandlers();
            foreach (auto id, ids) {
                Execution *newExecution = Execution::copy(execution, id);
                // todo: optimize execution by re-using the current execution.
                //       restart from initial state costs much.
                queue_.push_back(newExecution);
            }

            QString path = QDir::homePath() + QStringLiteral("/%1.jpg").arg(state);
            qDebug() << "Taking screenshot for state:" << state << path;
            execution->takeScreenshot(path);
        }

        delete execution;
        execution = nullptr;

        if (counter++ > 20) {
            qCritical() << "Aborting";
            return;
        }
    }

    qDebug() << "Executed" << counter << "paths";
}
