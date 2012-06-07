#-------------------------------------------------
#
# Project created by QtCreator 2012-05-25T11:58:59
#
#-------------------------------------------------

QT       += core gui

TARGET = pic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    storycardscene.cpp \
    setupdialog.cpp

HEADERS  += mainwindow.h \
    storycardscene.h \
    setupdialog.h

FORMS    += mainwindow.ui \
    setupdialog.ui

INCLUDEPATH += ./libmaia
LIBS += ./libmaia/debug/libmaia.a
QT   += xml network
