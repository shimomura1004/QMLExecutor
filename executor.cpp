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
    while (!queue_.isEmpty()) {
        Execution *execution = queue_.takeFirst();
        execution->execute();

        auto f = [](QQmlApplicationEngine *engine){
            auto state = engine->rootObjects().first()->property("state").toString();
            auto substate = engine->rootObjects().first()->property("substate").toBool();
            return state + substate;
        };
        auto state = execution->getState<QString>(f);

        if (!reachedState_.contains(state)) {
            reachedState_.insert(state);

            auto ids = execution->getInvokableEventHandlers();
            foreach (auto id, ids) {
                Execution *newExecution = Execution::copy(execution, id);
                queue_.push_back(newExecution);
            }

            static int count = 0;
            QString path = QDir::homePath() + QStringLiteral("/hoge%1.jpg").arg(count);
            execution->takeScreenshot(path);
            count++;
        }

        delete execution;
        execution = nullptr;

        if (queue_.length() > 20) {
            qCritical() << "To many state";
            return;
        }
    }
}
