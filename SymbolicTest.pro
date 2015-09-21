TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    execution.cpp \
    executor.cpp \
    helper.cpp \
    finders/clickableobjectfinder.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    execution.h \
    executor.h \
    helper.h \
    finders/clickableobjectfinder.h

INCLUDEPATH += \
    ../../../Qt/5.4/clang_64/lib/QtQml.framework/Versions/5/Headers/5.4.2/QtQml \
    ../../../Qt/5.4/clang_64/lib/QtQuick.framework/Versions/5/Headers/5.4.2/QtQuick \
    ../../../Qt/5.4/clang_64/lib/QtCore.framework/Versions/5/Headers/5.4.2/QtCore
