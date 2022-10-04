/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <graphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QLabel *labViewSize;
    QLabel *LabSceneRect;
    GraphicsView *graphicsView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(50, 10, 641, 91));
        labViewSize = new QLabel(groupBox);
        labViewSize->setObjectName(QString::fromUtf8("labViewSize"));
        labViewSize->setGeometry(QRect(20, 20, 565, 23));
        LabSceneRect = new QLabel(groupBox);
        LabSceneRect->setObjectName(QString::fromUtf8("LabSceneRect"));
        LabSceneRect->setGeometry(QRect(40, 60, 565, 23));
        graphicsView = new GraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(40, 170, 721, 351));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QString());
        labViewSize->setText(QCoreApplication::translate("MainWindow", "Graphics View\345\235\220\346\240\207\357\274\214\345\267\246\344\270\212\350\247\222\346\200\273\346\230\257\357\274\2100,0\357\274\211\357\274\214\345\256\275\345\272\246=\357\274\214\351\225\277\345\272\246=", nullptr));
        LabSceneRect->setText(QCoreApplication::translate("MainWindow", "QGraphicsView::sceneRect=()", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
