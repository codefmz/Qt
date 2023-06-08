#ifndef THREADSAVEFILE_H
#define THREADSAVEFILE_H

#include <QThread>


class ThreadSaveFile:public QThread
{
public:
    explicit ThreadSaveFile(QObject *parent = nullptr);
    void stopThread();
private:
    bool isStopped;
    void run() Q_DECL_OVERRIDE;
};

#endif // THREADSAVEFILE_H
