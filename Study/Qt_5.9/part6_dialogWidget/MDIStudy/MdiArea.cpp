#include "MdiArea.h"
#include <QDebug>
#include <QFormDoc.h>

MdiArea::MdiArea()
{
    connect(this, SIGNAL(subWindowActivated(QMdiSubWindow*)), SLOT(on_mdiArea_subWindowActivated(QMdiSubWindow *)));
}

bool MdiArea::eventFilter(QObject *watched, QEvent *event)
{

    if(watched == this->findChildren<QTabBar*>().at(0))
    {
            qDebug() << "MdiArea::eventFilter";
            if(event->type() == QEvent::MouseButtonPress)
            {
                QTabBar * tab = this->findChildren<QTabBar*>().at(0);
                if(tab->tabText(tab->currentIndex()).toInt() > 5)
                {
                    event->accept();
                    return true;
                }


            }
    }
    return QMdiArea::eventFilter(watched,event);
}





void MdiArea::on_mdiArea_subWindowActivated(QMdiSubWindow *subWindow)
{
    qDebug()<< "MainWindow::on_mdiArea_subWindowActivated";
////    if(mdiArea->subWindowList().count() == 0)
////    {
////        ui->actCopy->setEnabled(false);
////        ui->actCut->setEnabled(false);
////        ui->actFont->setEnabled(false);
////        ui->actPaste->setEnabled(false);
////        ui->statusbar->clearMessage();
////    }else
////    {
////        QFormDoc * formDoc = static_cast<QFormDoc *>(mdiArea->activeSubWindow()->widget());
////        ui->statusbar->showMessage(formDoc->currentFileName());
////    }

//    QList<QTabBar*>
//    QTabBar * tabBar = this->findChildren<QTabBar*>().at(0);

//    if(tabBar!= nullptr)
//    {
//        qDebug()<< "MainWindow::on_mdiArea_subWindowActivated -- tabBar is not nullptr";
//    }
//    if(subWindow != nullptr && lastActiceSubWindow != nullptr && subWindow != lastActiceSubWindow)
//    {
//        int before = lastActiceSubWindow->windowTitle().toInt();
//        if(cur < before)
//        {
////            mdiArea->setActiveSubWindow(lastActiceSubWindow);
////            mdiArea->removeSubWindow(subWindow);
////            lastActiceSubWindow->setWindowState(Qt::WindowFullScreen);
////            QFormDoc * formDoc = static_cast<QFormDoc*>(lastActiceSubWindow->window());
////            formDoc->showMaximized();

//            subWindow->hide();
//            lastActiceSubWindow->showMaximized();
//            qDebug() << "cur = " << cur;
//            qDebug() << " before = " << before;
//            return;
//        }
//    }
//    lastActiceSubWindow = subWindow;
}
