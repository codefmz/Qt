#include "qdicethread.h"
#include <QTime>
#include <QRandomGenerator>

QRandomGenerator randGen = QRandomGenerator::securelySeeded();
QDiceThread::QDiceThread() {}

void QDiceThread::diceBegin()
{
    m_paused = false;
}

void QDiceThread::diceEnd()
{
    m_paused = true;
}

void QDiceThread::stopThread()
{
    m_stop = true;
}

bool QDiceThread::readValue(int* seq, int* diceValue)
{
    if (mutex.tryLock())
    {
        *seq = m_seq;
        *diceValue = m_diceValue;
        mutex.unlock();
        return true;
    }
    return false;
}

void QDiceThread::run()
{
    m_stop = false; //启动线程时令m_stop=false
    m_seq = 0;

    while (!m_stop) //循环主体
    {
        if (!m_paused)
        {
            mutex.lock();
            m_diceValue = randGen.bounded(6) + 1;
            m_seq++;
            mutex.unlock();
        }
        msleep(500); //线程休眠100ms
    }
}
