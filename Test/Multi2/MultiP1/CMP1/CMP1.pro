QT -= gui

TEMPLATE = lib #表示项目是共享库

TARGET = CMP1 #表示编译后的库名字为 CMP1
DEFINES += CMP1_LIBRARY # 项目所需要的额外的预处理程序定义的列表

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CMP1.cpp

HEADERS += \
    CMP1_global.h \
    CMP1.h

# Default rules for deployment.
unix { #表示Linux环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DESTDIR = ../../lib #定义项目编译之后生成的结果文件的存放路径

win32 { #表示Windows环境的特殊设置，这个大括号不能单独放在下一行，否则编译会报错

}


