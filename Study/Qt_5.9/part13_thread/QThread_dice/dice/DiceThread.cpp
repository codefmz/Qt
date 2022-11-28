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

void DiceThread::run()
{
    m_stop = false;
    m_seq = 0;
    qsrand(QTime::currentTime().msec());//随机数初始化，qsrand是线程安全的
    while(!m_stop)
    {
       if(!m_paused)
       {
          m_diceValue = qrand() % 6 + 1;
          m_seq++;

          emit newValued(m_seq, m_diceValue);
       }
       msleep(50); //线程休眠500ms
    }
    quit();
}
