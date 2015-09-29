TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    executor.cpp \
    helper.cpp \
    finders/clickableobjectfinder.cpp \
    executionbase.cpp \
    execution.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    executor.h \
    helper.h \
    finders/clickableobjectfinder.h \
    executionqueue.h \
    executionbase.h \
    execution.h

QMAKE_CXXFLAGS += \
    -isystem ../../../Qt/5.5/clang_64/lib/QtQml.framework/Versions/5/Headers/5.5.0/QtQml \
    -isystem ../../../Qt/5.5/clang_64/lib/QtQuick.framework/Versions/5/Headers/5.5.0/QtQuick \
    -isystem ../../../Qt/5.5/clang_64/lib/QtCore.framework/Versions/5/Headers/5.5.0/QtCore
