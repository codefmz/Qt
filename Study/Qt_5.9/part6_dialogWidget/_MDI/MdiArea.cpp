#include "MdiArea.h"
#include <QDebug>
#include <QFormDoc.h>

MdiArea::MdiArea()
{
    tabBar = this->findChildren<QTabBar*>().at(0);
//    if(tabBar)
//    {
//        tabBar->setTabsClosable(true);
//    }
}

bool MdiArea::eventFilter(QObject *watched, QEvent *event)
{

    if(watched == tabBar)
    {
            qDebug() << "MdiArea::eventFilter";
            if(event->type() == QEvent::MouseButtonPress)
            {
                if(tabBar->tabText(tabBar->currentIndex()).toInt() > 5)
                {
                    event->accept();
                    return true;
                }
            }
    }
    return QMdiArea::eventFilter(watched,event);
}

