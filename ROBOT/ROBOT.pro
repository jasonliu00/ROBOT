#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T14:43:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ROBOT
TEMPLATE = app


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
    arrow.cpp

HEADERS  += robotmainwindow.h \
    mygraphicitem.h \
    mygraphicsellipseitem.h \
    mygraphicspolygonitem.h \
    mygraphicsscene.h \
    graphicwidget.h \
    motorstartdialog.h \
    mycgqitem.h \
    myzxqitem.h \
    arrow.h

FORMS    += robotmainwindow.ui \
    graphicwidget.ui \
    motorstartdialog.ui

RESOURCES += \
    robotimages.qrc
