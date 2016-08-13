#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T08:35:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = B-Spline
TEMPLATE = app


SOURCES += main.cpp\
        Marker.cpp\
        PixelPainter.cpp\
        MainWindow.cpp \
    Canvas.cpp

HEADERS  += Marker.h\
            PixelPainter.h\
            MainWindow.h \
    Canvas.h

FORMS    += MainWindow.ui
