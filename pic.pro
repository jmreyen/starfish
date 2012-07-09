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
    sprintmodel.cpp \
    sprintdata.cpp \
    burndownscene.cpp \
    storymodel.cpp \
    storyreport.cpp \
    storyreportdialog.cpp \
    tracdataloader.cpp \
    newstorydialog.cpp \
    storydata.cpp \
    storyitem.cpp \
    storyitemmodel.cpp

HEADERS  += mainwindow.h \
    storycardscene.h \
    setupdialog.h \
    sprintmodel.h \
    sprintdata.h \
    burndownscene.h \
    storydata.h \
    storymodel.h \
    storyreport.h \
    storyreportdialog.h \
    tracdataloader.h \
    abstractdataloader.h \
    newstorydialog.h \
    storyitem.h \
    storyitemmodel.h

FORMS    += mainwindow.ui \
    setupdialog.ui \
    reportdialog.ui \
    newstorydialog.ui

INCLUDEPATH += ./libmaia
LIBS += ./libmaia/debug/libmaia.a
QT   += xml network

RESOURCES += \
    images.qrc
