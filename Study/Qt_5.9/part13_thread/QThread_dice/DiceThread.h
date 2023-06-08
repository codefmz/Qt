#ifndef DICETHREAD_H
#define DICETHREAD_H

#include <QThread>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>
#include "QDebug"
#include <QRandomGenerator>
class DiceThread : public QThread
{
    Q_OBJECT
public:
    explicit DiceThread();
private:
    int m_seq = 0;
    int m_diceValue;
    bool m_paused = true;
    bool m_stop = false;
    QRandomGenerator randGen;
public:
    void diceBegin();
    void dicePause();
    void stopThread();
protected:
    void run() Q_DECL_OVERRIDE;
signals:
    void newValued(int seq, int diceValue); //使用信号传递值
};

#endif // DICETHREAD_H
