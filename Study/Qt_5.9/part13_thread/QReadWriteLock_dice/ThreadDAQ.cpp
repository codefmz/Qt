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
    while (!isStopped)
    {
        Lock.lockForWrite();
        for (auto& c : buffer)
        {
            c = randGen.bounded(6) + 1;
        }
        Lock.unlock();
        msleep(500);
    }
    quit();
}
