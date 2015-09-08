TEMPLATE = app

QT += qml quick widgets 

CONFIG += release qml_release

#Additional import path used to resolve QML modules in Qt Creator code model
QML_IMPORT_PATH =

TEMPLATE = app
TARGET = QAxisCurve

UI_DIR = ../bin
DESTDIR = ../bin
RCC_DIR = ../bin
MOC_DIR = ../bin
OBJECTS_DIR = ../bin

RESOURCES += qml.qrc

include(./QAxis/QAxis.pri)

SOURCES += main.cpp
