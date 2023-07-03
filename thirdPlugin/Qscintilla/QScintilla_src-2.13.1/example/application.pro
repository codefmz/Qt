
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG      += qscintilla2

# Comment this in to build a dynamic library supporting multiple architectures
# on macOS.
#QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64

HEADERS      = mainwindow.h
SOURCES      = main.cpp mainwindow.cpp
RESOURCES    = application.qrc
