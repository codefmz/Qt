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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <graphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actItem_Rect;
    QAction *actItem_Ellipse;
    QAction *actItem_Line;
    QAction *actEdit_Delete;
    QAction *actQuit;
    QAction *actItem_Text;
    QAction *actEdit_Front;
    QAction *actEdit_Back;
    QAction *actItem_Polygon;
    QAction *actZoomIn;
    QAction *actZoomOut;
    QAction *actRotateLeft;
    QAction *actRotateRight;
    QAction *actRestore;
    QAction *actGroup;
    QAction *actGroupBreak;
    QAction *actItem_Circle;
    QAction *actItem_Triangle;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    GraphicsView *graphicsView;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(739, 527);
        MainWindow->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actItem_Rect = new QAction(MainWindow);
        actItem_Rect->setObjectName(QString::fromUtf8("actItem_Rect"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/RECTANGL.BMP"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Rect->setIcon(icon);
        actItem_Ellipse = new QAction(MainWindow);
        actItem_Ellipse->setObjectName(QString::fromUtf8("actItem_Ellipse"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/ELLIPSE.BMP"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Ellipse->setIcon(icon1);
        actItem_Line = new QAction(MainWindow);
        actItem_Line->setObjectName(QString::fromUtf8("actItem_Line"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/LINE.BMP"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Line->setIcon(icon2);
        actEdit_Delete = new QAction(MainWindow);
        actEdit_Delete->setObjectName(QString::fromUtf8("actEdit_Delete"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/images/108.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actEdit_Delete->setIcon(icon3);
        actQuit = new QAction(MainWindow);
        actQuit->setObjectName(QString::fromUtf8("actQuit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/images/132.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actQuit->setIcon(icon4);
        actItem_Text = new QAction(MainWindow);
        actItem_Text->setObjectName(QString::fromUtf8("actItem_Text"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/images/800.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Text->setIcon(icon5);
        actEdit_Front = new QAction(MainWindow);
        actEdit_Front->setObjectName(QString::fromUtf8("actEdit_Front"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/images/528.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actEdit_Front->setIcon(icon6);
        actEdit_Back = new QAction(MainWindow);
        actEdit_Back->setObjectName(QString::fromUtf8("actEdit_Back"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/images/526.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actEdit_Back->setIcon(icon7);
        actItem_Polygon = new QAction(MainWindow);
        actItem_Polygon->setObjectName(QString::fromUtf8("actItem_Polygon"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/images/FREEFORM.BMP"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Polygon->setIcon(icon8);
        actZoomIn = new QAction(MainWindow);
        actZoomIn->setObjectName(QString::fromUtf8("actZoomIn"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/images/zoomin.png"), QSize(), QIcon::Normal, QIcon::Off);
        actZoomIn->setIcon(icon9);
        actZoomOut = new QAction(MainWindow);
        actZoomOut->setObjectName(QString::fromUtf8("actZoomOut"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/images/zoomout.png"), QSize(), QIcon::Normal, QIcon::Off);
        actZoomOut->setIcon(icon10);
        actRotateLeft = new QAction(MainWindow);
        actRotateLeft->setObjectName(QString::fromUtf8("actRotateLeft"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/images/rotateleft.png"), QSize(), QIcon::Normal, QIcon::Off);
        actRotateLeft->setIcon(icon11);
        actRotateRight = new QAction(MainWindow);
        actRotateRight->setObjectName(QString::fromUtf8("actRotateRight"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/images/rotateright.png"), QSize(), QIcon::Normal, QIcon::Off);
        actRotateRight->setIcon(icon12);
        actRestore = new QAction(MainWindow);
        actRestore->setObjectName(QString::fromUtf8("actRestore"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/images/420.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actRestore->setIcon(icon13);
        actGroup = new QAction(MainWindow);
        actGroup->setObjectName(QString::fromUtf8("actGroup"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/images/UNGROUP.BMP"), QSize(), QIcon::Normal, QIcon::Off);
        actGroup->setIcon(icon14);
        actGroupBreak = new QAction(MainWindow);
        actGroupBreak->setObjectName(QString::fromUtf8("actGroupBreak"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/images/images/128.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actGroupBreak->setIcon(icon15);
        actItem_Circle = new QAction(MainWindow);
        actItem_Circle->setObjectName(QString::fromUtf8("actItem_Circle"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/images/images/08.JPG"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Circle->setIcon(icon16);
        actItem_Triangle = new QAction(MainWindow);
        actItem_Triangle->setObjectName(QString::fromUtf8("actItem_Triangle"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/images/images/Icon1242.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actItem_Triangle->setIcon(icon17);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new GraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar_2);

        toolBar->addAction(actZoomIn);
        toolBar->addAction(actZoomOut);
        toolBar->addAction(actRestore);
        toolBar->addSeparator();
        toolBar->addAction(actRotateLeft);
        toolBar->addAction(actRotateRight);
        toolBar->addAction(actEdit_Front);
        toolBar->addAction(actEdit_Back);
        toolBar->addSeparator();
        toolBar->addAction(actGroup);
        toolBar->addAction(actGroupBreak);
        toolBar->addAction(actEdit_Delete);
        toolBar->addAction(actQuit);
        toolBar_2->addAction(actItem_Rect);
        toolBar_2->addAction(actItem_Ellipse);
        toolBar_2->addAction(actItem_Circle);
        toolBar_2->addAction(actItem_Triangle);
        toolBar_2->addAction(actItem_Polygon);
        toolBar_2->addAction(actItem_Line);
        toolBar_2->addAction(actItem_Text);

        retranslateUi(MainWindow);
        QObject::connect(actQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\347\273\230\345\233\276\350\275\257\344\273\266", nullptr));
        actItem_Rect->setText(QCoreApplication::translate("MainWindow", "\347\237\251\345\275\242", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Rect->setToolTip(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\237\251\345\275\242", nullptr));
#endif // QT_CONFIG(tooltip)
        actItem_Ellipse->setText(QCoreApplication::translate("MainWindow", "\346\244\255\345\234\206", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Ellipse->setToolTip(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\244\255\345\234\206\345\236\213", nullptr));
#endif // QT_CONFIG(tooltip)
        actItem_Line->setText(QCoreApplication::translate("MainWindow", "\347\233\264\347\272\277", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Line->setToolTip(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\233\264\347\272\277", nullptr));
#endif // QT_CONFIG(tooltip)
        actEdit_Delete->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
#if QT_CONFIG(tooltip)
        actEdit_Delete->setToolTip(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\351\200\211\344\270\255\347\232\204\345\233\276\345\205\203", nullptr));
#endif // QT_CONFIG(tooltip)
        actQuit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(tooltip)
        actQuit->setToolTip(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\346\234\254\347\263\273\347\273\237", nullptr));
#endif // QT_CONFIG(tooltip)
        actItem_Text->setText(QCoreApplication::translate("MainWindow", "\346\226\207\345\255\227", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Text->setToolTip(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\226\207\345\255\227", nullptr));
#endif // QT_CONFIG(tooltip)
        actEdit_Front->setText(QCoreApplication::translate("MainWindow", "\345\211\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        actEdit_Front->setToolTip(QCoreApplication::translate("MainWindow", "\345\261\205\344\272\216\346\234\200\345\211\215\351\235\242", nullptr));
#endif // QT_CONFIG(tooltip)
        actEdit_Back->setText(QCoreApplication::translate("MainWindow", "\345\220\216\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        actEdit_Back->setToolTip(QCoreApplication::translate("MainWindow", "\345\261\205\344\272\216\346\234\200\345\220\216\351\235\242", nullptr));
#endif // QT_CONFIG(tooltip)
        actItem_Polygon->setText(QCoreApplication::translate("MainWindow", "\346\242\257\345\275\242", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Polygon->setToolTip(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\242\257\345\275\242", nullptr));
#endif // QT_CONFIG(tooltip)
        actZoomIn->setText(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
#if QT_CONFIG(tooltip)
        actZoomIn->setToolTip(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
#endif // QT_CONFIG(tooltip)
        actZoomOut->setText(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
#if QT_CONFIG(tooltip)
        actZoomOut->setToolTip(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        actRotateLeft->setText(QCoreApplication::translate("MainWindow", "\345\267\246\346\227\213\350\275\254", nullptr));
#if QT_CONFIG(tooltip)
        actRotateLeft->setToolTip(QCoreApplication::translate("MainWindow", "\345\267\246\346\227\213\350\275\254", nullptr));
#endif // QT_CONFIG(tooltip)
        actRotateRight->setText(QCoreApplication::translate("MainWindow", "\345\217\263\346\227\213\350\275\254", nullptr));
#if QT_CONFIG(tooltip)
        actRotateRight->setToolTip(QCoreApplication::translate("MainWindow", "\345\217\263\346\227\213\350\275\254", nullptr));
#endif // QT_CONFIG(tooltip)
        actRestore->setText(QCoreApplication::translate("MainWindow", "\346\201\242\345\244\215", nullptr));
#if QT_CONFIG(tooltip)
        actRestore->setToolTip(QCoreApplication::translate("MainWindow", "\346\201\242\345\244\215\345\244\247\345\260\217", nullptr));
#endif // QT_CONFIG(tooltip)
        actGroup->setText(QCoreApplication::translate("MainWindow", "\347\273\204\345\220\210", nullptr));
#if QT_CONFIG(tooltip)
        actGroup->setToolTip(QCoreApplication::translate("MainWindow", "\347\273\204\345\220\210", nullptr));
#endif // QT_CONFIG(tooltip)
        actGroupBreak->setText(QCoreApplication::translate("MainWindow", "\346\211\223\346\225\243", nullptr));
#if QT_CONFIG(tooltip)
        actGroupBreak->setToolTip(QCoreApplication::translate("MainWindow", "\345\217\226\346\266\210\347\273\204\345\220\210", nullptr));
#endif // QT_CONFIG(tooltip)
        actItem_Circle->setText(QCoreApplication::translate("MainWindow", "\345\234\206\345\275\242", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Circle->setToolTip(QCoreApplication::translate("MainWindow", "\345\234\206\345\275\242", nullptr));
#endif // QT_CONFIG(tooltip)
        actItem_Triangle->setText(QCoreApplication::translate("MainWindow", "\344\270\211\350\247\222\345\275\242", nullptr));
#if QT_CONFIG(tooltip)
        actItem_Triangle->setToolTip(QCoreApplication::translate("MainWindow", "\344\270\211\350\247\222\345\275\242", nullptr));
#endif // QT_CONFIG(tooltip)
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        toolBar_2->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
