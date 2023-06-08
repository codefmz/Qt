#ifndef THREADDAQ_H
#define THREADDAQ_H

#include <QThread>
#include <QRandomGenerator>
class ThreadDAQ : public QThread
{
public:
    explicit ThreadDAQ(QObject *parent = nullptr);
    void stopThread();
private:
    bool isStopped;
    QRandomGenerator randGen;
    void run() Q_DECL_OVERRIDE;
};

#endif // THREADDAQ_H
