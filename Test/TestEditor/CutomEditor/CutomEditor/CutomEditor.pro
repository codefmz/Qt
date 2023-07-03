QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Editor.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    Editor.h \
    MainWindow.h

# Default rules for deployment.
TARGET = CustomEdit

DESTDIR = ../bin

UI_DIR = generatedfiles/ui

MOC_DIR = generatedfiles/moc

RCC_DIR = generatedfiles/rcc

ICON = Resources/icons/edit.ico

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

RESOURCES += \
    res.qrc
