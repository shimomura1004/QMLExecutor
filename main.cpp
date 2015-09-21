#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "executor.h"

#include "finders/clickableobjectfinder.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine(QUrl("qrc:/NestedView.qml"));
    ClickableObjectFinder::find(engine.rootObjects().first());
    return app.exec();

//    Executor executor(QUrl("qrc:/main.qml"));
//    executor.execute();

//    return 0;
}
