#include "DiceThread.h"
DiceThread::DiceThread()
{

}

void DiceThread::diceBegin()
{
    m_paused = false;
}

void DiceThread::dicePause()
{
    m_paused = true;

}

void DiceThread::stopThread()
{
    m_stop = true;
}

bool DiceThread::readValue(int *tmpSeq, int *tmpValue)
{
    if(mutex.tryLock())
    {
        *tmpSeq = m_seq;
        *tmpValue = m_diceValue;
        mutex.unlock();
        return true;
    }
    return false;
}

void DiceThread::run()
{
    m_stop = false;
    m_seq = 0;
    qsrand(QTime::currentTime().msec());//随机数初始化，qsrand是线程安全的
    while(!m_stop)
    {
       if(!m_paused)
       {
          QMutexLocker locker(&mutex);
          m_diceValue = qrand() % 6 + 1;
          m_seq++;
          qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ <<" -- ";
          emit newValued(m_seq, m_diceValue);
          qDebug().nospace() << __FILE__ << __LINE__ << __FUNCTION__ <<" -- ";
       }
       msleep(50); //线程休眠500ms
    }
    quit();
}
