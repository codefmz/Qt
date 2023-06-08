#include "qmythread.h"
#include <QMutex>
#include <QTime>
#include <QWaitCondition>
#include <QRandomGenerator>
#include <QDebug>
QMutex         mutex;
QWaitCondition newdataAvailable;
int seq = 0; //序号
int diceValue;
QRandomGenerator randGen = QRandomGenerator::securelySeeded();
QThreadProducer::QThreadProducer() {}

void QThreadProducer::stopThread()
{
    QMutexLocker locker(&mutex);
    m_stop = true;
}

void QThreadProducer::run()
{
    m_stop = false; //启动线程时令m_stop=false
    seq = 0;
    while (!m_stop) //循环主体
    {
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- gen out lock";
        mutex.lock();
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- gen in lock";
        diceValue = randGen.bounded(6) + 1;
        ++seq;
        mutex.unlock();
        newdataAvailable.wakeAll(); //唤醒所有线程，有新数据了
        msleep(100);                //线程休眠100ms
    }
}

QThreadConsumer::QThreadConsumer() {}

void QThreadConsumer::run()
{
    m_stop = false; //启动线程时令m_stop=false
    while (!m_stop) //循环主体
    {
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- out lock";
        mutex.lock();
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- " << " in lock";
        newdataAvailable.wait(&mutex); //会先解锁mutex，使其他线程可以使用mutex
        emit newValue(seq, diceValue);
        mutex.unlock();
    }
}

void QThreadConsumer::stopThread()
{
    QMutexLocker locker(&mutex);
    m_stop = true;
}
