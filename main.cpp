#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "executor.h"

#include <QDebug>

#include "finders/clickableobjectfinder.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //QQmlApplicationEngine engine(QUrl("qrc:/NestedView.qml"));
//    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
//    auto item = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
//    qDebug() << ClickableObjectFinder::find(item);
//    return app.exec();

    //Executor executor(QUrl("qrc:/main.qml"));
    //Executor executor(QUrl("qrc:/NestedView.qml"));
    Executor executor(QUrl("qrc:/HandlertestView.qml"));
    executor.execute();

    return 0;
}
