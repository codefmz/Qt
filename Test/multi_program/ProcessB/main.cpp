#include <QApplication>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>
struct CustomData
{
    QString processName;
    int     command;
    // 添加其他自定义数据成员
};
const QString keyA2B = "SharedMemory_A_B";
QSharedMemory sharedMemoryA(keyA2B);
const QString keyB2A = "SharedMemory_B_A";
QSharedMemory sharedMemoryB(keyB2A);
void          run(const QMap<int, QPixmap>& intPixMap)
{
    while (true)
    {
        QSystemSemaphore semaphore("MySemaphore_A_B", 0, QSystemSemaphore::Open);
        // 等待信号量
        semaphore.acquire();
        if (!sharedMemoryA.attach())
        {
            break;
        }
        QBuffer     buffer;
        QDataStream in(&buffer);
        CustomData  customData;
        buffer.setData((char*)sharedMemoryA.constData(), sharedMemoryA.size());
        buffer.open(QBuffer::ReadOnly);
        in >> customData.processName;
        in >> customData.command;
        buffer.close();
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- customData.processName = " << customData.processName
                           << " customData.command  = " << customData.command;
        sharedMemoryB.detach();
        if (customData.command == 0)
        {
            sharedMemoryA.detach();
            return;
        }
        QBuffer buffer2;
        buffer2.open(QBuffer::ReadWrite);
        QDataStream out(&buffer2);
        out << intPixMap[customData.command];
        int size = buffer2.size();
        if (!sharedMemoryB.create(size))
        {
            QSystemSemaphore semaphore2("MySemaphore_B_A", 0, QSystemSemaphore::Open);
            semaphore2.release();
            if (sharedMemoryA.isAttached())
            {
                if (!sharedMemoryA.detach())
                {
                    break;
                }
            }
            return;
        }
        char*       to = (char*)sharedMemoryB.data();
        const char* from = buffer2.data().data();
        memcpy(to, from, qMin(sharedMemoryB.size(), size));
        buffer2.close();
        QSystemSemaphore semaphore2("MySemaphore_B_A", 0, QSystemSemaphore::Open);
        semaphore2.release();
        sharedMemoryA.detach();
    }
}

int main(int argc, char* argv[])
{
    QApplication       a(argc, argv);
    QMap<int, QPixmap> intPixMap = { { 1, QPixmap(":/image/wallhaven-1ppv8v.png") }, { 2, QPixmap(":/image/wallhaven-5gygv3.jpg") },
                                     { 3, QPixmap(":/image/wallhaven-9ddqjw.png") }, { 4, QPixmap(":/image/wallhaven-kxxjgq.png") },
                                     { 5, QPixmap(":/image/wallhaven-yxqx6k.jpg") }, { 6, QPixmap(":/image/wallhaven-yxx2k7_2560x1080.png") } };
    run(intPixMap);
    return 0;
}
