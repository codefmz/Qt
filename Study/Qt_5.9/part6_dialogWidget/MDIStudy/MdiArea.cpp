#include "MdiArea.h"
#include <QDebug>
#include <QFormDoc.h>
MdiArea::MdiArea()
{

    connect(this, SIGNAL(subWindowActivated(QMdiSubWindow*)), SLOT(on_mdiArea_subWindowActivated(QMdiSubWindow *)));
}





void MdiArea::on_mdiArea_subWindowActivated(QMdiSubWindow *subWindow)
{
    qDebug()<< "MainWindow::on_mdiArea_subWindowActivated";
//    if(mdiArea->subWindowList().count() == 0)
//    {
//        ui->actCopy->setEnabled(false);
//        ui->actCut->setEnabled(false);
//        ui->actFont->setEnabled(false);
//        ui->actPaste->setEnabled(false);
//        ui->statusbar->clearMessage();
//    }else
//    {
//        QFormDoc * formDoc = static_cast<QFormDoc *>(mdiArea->activeSubWindow()->widget());
//        ui->statusbar->showMessage(formDoc->currentFileName());
//    }


    if(subWindow != nullptr && lastActiceSubWindow != nullptr && subWindow != lastActiceSubWindow)
    {
        int cur = subWindow->windowTitle().toInt();
        int before = lastActiceSubWindow->windowTitle().toInt();
        if(cur < before)
        {
//            mdiArea->setActiveSubWindow(lastActiceSubWindow);
//            mdiArea->removeSubWindow(subWindow);
//            lastActiceSubWindow->setWindowState(Qt::WindowFullScreen);
//            QFormDoc * formDoc = static_cast<QFormDoc*>(lastActiceSubWindow->window());
//            formDoc->showMaximized();

            subWindow->hide();
            lastActiceSubWindow->showMaximized();
            qDebug() << "cur = " << cur;
            qDebug() << " before = " << before;
            return;
        }
    }
    lastActiceSubWindow = subWindow;
}
