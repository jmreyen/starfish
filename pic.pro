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
    storyreport.cpp \
    storyreportdialog.cpp \
    newstorydialog.cpp \
    storyitem.cpp \
    storyitemmodel.cpp \
    storyitemdelegate.cpp \
    fields.cpp \
    io/iofactory.cpp \
    io/tracio.cpp

HEADERS  += mainwindow.h \
    storycardscene.h \
    setupdialog.h \
    sprintmodel.h \
    sprintdata.h \
    burndownscene.h \
    storyreport.h \
    storyreportdialog.h \
    newstorydialog.h \
    storyitem.h \
    storyitemmodel.h \
    storyitemdelegate.h \
    fields.h \
    io/abstractio.h \
    io/iofactory.h \
    io/tracio.h

FORMS    += mainwindow.ui \
    setupdialog.ui \
    reportdialog.ui \
    newstorydialog.ui

INCLUDEPATH += ./libmaia
LIBS += ./libmaia/debug/libmaia.a
QT   += xml network

RESOURCES += \
    images.qrc
