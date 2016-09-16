#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T09:19:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2D Transformations
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Canvas.cpp

HEADERS  += MainWindow.h \
    Canvas.h \
    Matrix.h

FORMS    += MainWindow.ui

RESOURCES += \
    resources.qrc
