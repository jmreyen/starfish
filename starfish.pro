#-------------------------------------------------
#
# Project created by QtCreator 2012-05-25T11:58:59
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = starfish
TEMPLATE = app


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    storyreport.cpp \
    io/iofactory.cpp \
    io/tracio.cpp \
    dialogs/storyreportdialog.cpp \
    dialogs/setupdialog.cpp \
    dialogs/newstorydialog.cpp \
    models/storymodel.cpp \
    models/storyitem.cpp \
    models/sprintmodel.cpp \
    models/sprintdata.cpp \
    views/storyitemdelegate.cpp \
    views/storycardscene.cpp \
    views/burndownscene.cpp \
    base/fields.cpp \
    models/storyproxymodel.cpp

HEADERS  += \
    mainwindow.h \
    storyreport.h \
    io/abstractio.h \
    io/iofactory.h \
    io/tracio.h \
   dialogs/storyreportdialog.h \
    dialogs/setupdialog.h \
    dialogs/newstorydialog.h \
    models/storymodel.h \
    models/storyitem.h \
    models/sprintmodel.h \
    models/sprintdata.h \
    views/storyitemdelegate.h \
    views/storycardscene.h \
    views/burndownscene.h \
    base/fields.h \
    models/storyproxymodel.h

FORMS    += \
    mainwindow.ui \
    dialogs/setupdialog.ui \
    dialogs/reportdialog.ui \
    dialogs/newstorydialog.ui \
    io/tracsettingsframe.ui

INCLUDEPATH += ./libmaia
LIBS += ./libmaia/debug/libmaia.a
QT   += xml network 

#RESOURCES += images.qrc

RESOURCES += \
    images.qrc
