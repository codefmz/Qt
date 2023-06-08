#include "ThreadDAQ.h"
#include "dialog.h"
ThreadDAQ::ThreadDAQ(QObject* parent)
    : QThread { parent }
{
    randGen = QRandomGenerator::securelySeeded();
}

void ThreadDAQ::stopThread()
{
    isStopped = true;
}

void ThreadDAQ::run()
{
    isStopped = false;
    int n = semBuf.available();
    if (n < 2)
    {
        semBuf.release(2 - n);
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- semBuf release n = " << n;
    }
    while (!isStopped)
    {
        semBuf.acquire();
        if (curNo == 1)
        {
            for (int num = 0; num < 10; ++num)
            {
                int c = randGen.bounded(6) + 1;
                buffer1.push_back(c);
            }
        }
        else
        {
            for (int num = 0; num < 10; ++num)
            {
                int c = randGen.bounded(6) + 1;
                buffer2.push_back(c);
            }
        }
        curNo = curNo == 1 ? 2 : 1;
        fullBuf.release();
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- fullBuf release";
    }
    quit();
}
