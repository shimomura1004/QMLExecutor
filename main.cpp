#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "helper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
//    return app.exec();

    Helper executor(QUrl("qrc:/main.qml"));
    executor.run();

    return 0;
}
