#include "ThreadSaveFile.h"
#include <QDir>
#include <QTextStream>
#include <dialog.h>

ThreadSaveFile::ThreadSaveFile(QObject* parent)
    : QThread(parent)
{
    isStopped = false;
}

void ThreadSaveFile::stopThread()
{
    isStopped = true;
}

void ThreadSaveFile::run()
{
    isStopped = false;
    QString filePath = QDir().currentPath() + "//buffer.txt";
    QFile   file(filePath);
    file.open(QIODevice::WriteOnly);
    QTextStream textStream(&file);
    while (!isStopped)
    {
        Lock.lockForRead();
        foreach (auto num, buffer)
        {
            textStream << num << "  ";
        }
        textStream << Qt::endl;
        Lock.unlock();
        msleep(500);
        file.flush();
    }
    file.close();
    quit();
}
