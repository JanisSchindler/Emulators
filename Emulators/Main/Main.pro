#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T18:41:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Main
TEMPLATE = app

INCLUDEPATH += ..\Input\

SOURCES += main.cpp\
        mainwindow.cpp \
    controllerinput.cpp

HEADERS  += mainwindow.h\
    controllerinput.h


FORMS    += mainwindow.ui
