#include "ThreadShow.h"
#include <dialog.h>

ThreadShow::ThreadShow(QObject* parent)
    : QThread { parent }
{
}

void ThreadShow::stopThread()
{
    isStopped = true;
}

void ThreadShow::run()
{
    isStopped = false;
    while (!isStopped)
    {
        Lock.lockForRead();
        foreach (auto num, buffer)
        {
//            qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << num;

            if(isStopped)
            {
                break;
            }
            emit newValued(num);
            msleep(50);
        }
        Lock.unlock();
    }
    quit();
}
