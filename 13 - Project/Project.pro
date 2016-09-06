#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T15:29:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app
INCLUDEPATH += .

SOURCES += main.cpp\
        MainWindow.cpp \
		Canvas.cpp \
		PixelPainter.cpp \
    OBJ.cpp \
    Triangle.cpp

HEADERS  += MainWindow.h \
			Canvas.h \
			PixelPainter.h \
    OBJ.h \
    Triangle.h

FORMS    += MainWindow.ui

RESOURCES += res.qrc
