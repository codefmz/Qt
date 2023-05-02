#-------------------------------------------------
#
# Project created by QtCreator 2016-11-19T10:58:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = samp4_9
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qwintspindelegate.cpp

HEADERS  += mainwindow.h \
    qwintspindelegate.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
