#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T18:41:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Main
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    controllerinput.cpp \
    viewmodel.cpp \
    mocmodel.cpp

HEADERS  += mainwindow.h\
    controllerinput.h \
    viewmodel.h \
    mocmodel.h


FORMS    += mainwindow.ui
