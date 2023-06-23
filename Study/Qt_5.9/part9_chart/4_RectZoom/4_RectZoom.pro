#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T13:17:47
#
#-------------------------------------------------

QT       += core gui

QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = samp9_4
TEMPLATE = app

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

SOURCES += main.cpp\
        mainwindow.cpp \
    qwchartview.cpp

HEADERS  += mainwindow.h \
    qwchartview.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
