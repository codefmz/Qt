#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T20:08:33
#
#-------------------------------------------------

QT       += core gui

QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QueryEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wdialogdata.cpp

HEADERS  += mainwindow.h \
    wdialogdata.h

FORMS    += mainwindow.ui \
    wdialogdata.ui

RESOURCES += \
    res.qrc

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
