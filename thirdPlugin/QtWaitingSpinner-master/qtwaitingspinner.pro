QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
#CONFIG += staticlib
TARGET = qtwaitingspinner

SOURCES += \
    main.cpp \
    waitingspinnerwidget.cpp
    
HEADERS += \
    waitingspinnerwidget.h
