#include "DiceThread.h"

DiceThread::DiceThread()
{

}

void DiceThread::diceBegin()
{

}

void DiceThread::dicePause()
{
    m_paused = true;

}

void DiceThread::stopThread()
{
    m_paused = true;
}

void DiceThread::run()
{
    m_stop = false;
    m_seq = 0;
    qsrand(QTime::currentTime().second());
    while(!m_stop)
    {
       if(!m_paused)
       {
          m_diceValue = qrand() % 6 + 1;
          m_seq++;
          emit newValued(m_seq, m_diceValue);
       }
    }
    quit();
}
