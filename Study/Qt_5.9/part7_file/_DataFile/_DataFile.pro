#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T23:36:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = samp7_2
TEMPLATE = app
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

SOURCES += main.cpp\
        mainwindow.cpp \
    qwintspindelegate.cpp \
    qwfloatspindelegate.cpp \
    qwcomboboxdelegate.cpp

HEADERS  += mainwindow.h \
    qwintspindelegate.h \
    qwfloatspindelegate.h \
    qwcomboboxdelegate.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
