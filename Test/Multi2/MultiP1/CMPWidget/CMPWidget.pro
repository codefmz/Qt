QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TEMPLATE = app #表示项目是应用程序模式



win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    LIBS += ../../lib/CMP1.lib #包含项目需要用到的共享库
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ../CMP1

DESTDIR = ../../app

include(CMPWidget.pri)
