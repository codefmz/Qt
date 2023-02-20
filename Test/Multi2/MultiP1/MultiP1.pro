TEMPLATE = subdirs

SUBDIRS += \
    QT_1_SharedMemory\
    CMP1 \
    CMPWidget\

CONFIG += ordered #CONFIG选项要求各个子项目按顺序编译，子目录的编译顺序在SUBDIRS中指明
