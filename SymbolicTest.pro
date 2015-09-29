TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    helper.cpp \
    finders/clickableobjectfinder.cpp \
    execution/execution.cpp \
    execution/executionbase.cpp \
    execution/executor.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    helper.h \
    finders/clickableobjectfinder.h \
    execution/execution.h \
    execution/executionbase.h \
    execution/executor.h \
    execution/executionqueue.h

QMAKE_CXXFLAGS += \
    -isystem ../../../Qt/5.5/clang_64/lib/QtQml.framework/Versions/5/Headers/5.5.0/QtQml \
    -isystem ../../../Qt/5.5/clang_64/lib/QtQuick.framework/Versions/5/Headers/5.5.0/QtQuick \
    -isystem ../../../Qt/5.5/clang_64/lib/QtCore.framework/Versions/5/Headers/5.5.0/QtCore
