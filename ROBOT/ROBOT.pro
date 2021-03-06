#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T14:43:48
#
#-------------------------------------------------
#LIBS     += User32.LIB
QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ROBOT
TEMPLATE = app
OTHER_FILES += roboticon.rc
RC_FILE = roboticon.rc

SOURCES += main.cpp\
        robotmainwindow.cpp \
    mygraphicitem.cpp \
    mygraphicsellipseitem.cpp \
    mygraphicspolygonitem.cpp \
    mygraphicsscene.cpp \
    graphicwidget.cpp \
    motorstartdialog.cpp \
    mycgqitem.cpp \
    myzxqitem.cpp \
    arrow.cpp \
    startmotordialog.cpp \
    stopmotordialog.cpp \
    modelpropertydata.cpp \
    lightdialog.cpp \
    ringdialog.cpp \
    showdialog.cpp \
    delaydialog.cpp \
    mykzqitem.cpp \
    modelgraphicsitem.cpp \
    panduandialog.cpp

HEADERS  += robotmainwindow.h \
    mygraphicitem.h \
    mygraphicsellipseitem.h \
    mygraphicspolygonitem.h \
    mygraphicsscene.h \
    graphicwidget.h \
    motorstartdialog.h \
    mycgqitem.h \
    myzxqitem.h \
    arrow.h \
    itemtypes.h \
    startmotordialog.h \
    stopmotordialog.h \
    modelpropertydata.h \
    lightdialog.h \
    ringdialog.h \
    showdialog.h \
    delaydialog.h \
    mykzqitem.h \
    modelgraphicsitem.h \
    panduandialog.h

FORMS    += robotmainwindow.ui \
    graphicwidget.ui \
    motorstartdialog.ui \
    startmotordialog.ui \
    stopmotordialog.ui \
    lightdialog.ui \
    ringdialog.ui \
    showdialog.ui \
    delaydialog.ui \
    panduandialog.ui

RESOURCES += \
    robotimages.qrc

DISTFILES += \
    roboticon.rc


