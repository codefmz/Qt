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

//        foreach (auto num, buffer)
//        {
//            textStream << num << "  ";
//        }
//        textStream << Qt::endl;

        msleep(500);
        file.flush();
    }
    file.close();
    quit();
}
