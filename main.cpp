#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "executor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
//    return app.exec();

    Executor executor(QUrl("qrc:/main.qml"));
    executor.execute();

    return 0;
}
