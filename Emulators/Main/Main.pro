#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T18:41:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Emulators
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    controllerinput.cpp \
    viewmodel.cpp \
    mocmodel.cpp \
    loader.cpp \
    logger.cpp

HEADERS  += mainwindow.h\
    controllerinput.h \
    viewmodel.h \
    mocmodel.h \
    loader.h \
    logger.h


FORMS    += mainwindow.ui
