#ifndef THREADSHOW_H
#define THREADSHOW_H

#include <QThread>
#include <QDebug>
class ThreadShow : public QThread
{
    Q_OBJECT
public:
    explicit ThreadShow(QObject *parent = nullptr);
    void stopThread();
protected:
    bool isStopped;
    void run() Q_DECL_OVERRIDE;
signals:
    void newValued(int aNum); //使用信号传递值
};

#endif // THREADSHOW_H
