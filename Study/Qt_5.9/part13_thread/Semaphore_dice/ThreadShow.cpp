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
    int n = fullBuf.available();
    if (n > 0)
    {
        fullBuf.acquire(1);
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- fullBuf accquire n = " << n;
    }
    while (!isStopped)
    {
        QList<int> temp;
        fullBuf.acquire();
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- show ";
        if (curNo == 1)
        {
            temp = buffer2;
            buffer2.clear();
        }
        else
        {
            temp = buffer1;
            buffer1.clear();
        }
        foreach (auto& num, temp)
        {
            emit newValued(num);
            msleep(50);
            if(isStopped)
            {
                break;
            }
        }
        semBuf.release();
    }
    quit();
}
