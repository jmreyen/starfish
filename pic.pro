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
    setupdialog.cpp \
    ticketreport.cpp \
    ticketreportdialog.cpp \
    ticketmodel.cpp \
    sprintmodel.cpp \
    sprintdata.cpp

HEADERS  += mainwindow.h \
    storycardscene.h \
    setupdialog.h \
    ticketreport.h \
    ticketreportdialog.h \
    ticketmodel.h \
    ticketdata.h \
    sprintmodel.h \
    sprintdata.h

FORMS    += mainwindow.ui \
    setupdialog.ui \
    reportdialog.ui

INCLUDEPATH += ./libmaia
LIBS += ./libmaia/debug/libmaia.a
QT   += xml network
