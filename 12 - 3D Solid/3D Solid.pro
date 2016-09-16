#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T09:19:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D Solid
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Canvas.cpp \
    Cube.cpp \
    Triangle.cpp

HEADERS  += MainWindow.h \
    Canvas.h \
    Cube.h \
    Triangle.h

FORMS    += MainWindow.ui

RESOURCES += \
    resources.qrc
