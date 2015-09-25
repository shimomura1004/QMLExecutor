#include "execution.h"
#include "helper.h"
#include "finders/clickableobjectfinder.h"

//#include <private/qquickmousearea_p.h>
#include <private/qquickevents_p_p.h>

#include <QQmlContext>
#include <QQuickWindow>
#include <QDir>
#include <QDebug>

QUrl Execution::url_;

Execution::Execution(EventSequence eventSequence, QObject *parent)
    : QObject(parent)
    , engine_(nullptr)
    , eventSequence_(eventSequence)
{
}

Execution::~Execution()
{
}

Execution *Execution::copy(const Execution *execution, Execution::ID id)
{
    EventSequence seq = execution->consumedEventSequence_;
    seq.append(id);
    return new Execution(seq);
}

bool Execution::execute(ExecutionQueue<QString> *queue)
{
    engine_ = new QQmlApplicationEngine(url_, this);
    engine_->rootContext()->setContextProperty("Executor", this);

    while (!eventSequence_.empty()) {
        auto targetId = eventSequence_.takeFirst();
        consumedEventSequence_.push_back(targetId);

        if (auto obj = Helper::getObject(engine_, targetId)) {
            qCritical() << "Clicking:" << targetId;
            auto event = new QQuickMouseEvent(0, 0, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
            QMetaObject::invokeMethod(obj, "clicked", Q_ARG(QQuickMouseEvent*, event));
        }
        else {
            qCritical() << "Unknown ID:" << targetId;
        }
    }

    auto state = getState();

    if (queue->contains(state)) {
        return false;
    }

    // save screenshot
    QString path = QDir::homePath() + QStringLiteral("/%1.png").arg(state);
    qDebug() << "Taking screenshot for state:" << state << path;
    takeScreenshot(path);

    auto ids = getInvokableEventHandlers();
    if (ids.empty()) {
        return false;
    }

    // reusing this execution
    this->eventSequence_.push_back(ids.takeFirst());
    queue->unpop(this);

    // add to queue
    foreach (auto id, ids) {
        Execution *newExecution = Execution::copy(this, id);
        queue->push(state, newExecution);
    }

    return true;
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

QList<Execution::ID> Execution::getInvokableEventHandlers()
{
    QList<Execution::ID> ids;

    foreach (auto object, engine_->rootObjects()) {
        foreach (auto item, ClickableObjectFinder::find(qobject_cast<QQuickWindow*>(object))) {
            ids.push_back(Helper::getId(engine_, item));
        }
    }

    return ids;
}

bool Execution::branch(bool condition)
{
    qDebug() << Q_FUNC_INFO << "called branch";
    return condition;
}
