#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QVector>
#include <ThreadDAQ.h>
#include <ThreadSaveFile.h>
#include <ThreadShow.h>
extern QList<int> buffer1;
extern QList<int> buffer2;
extern QSemaphore semBuf;
extern QSemaphore fullBuf;
extern int        curNo;
QT_BEGIN_NAMESPACE
namespace Ui
{
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget* parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog*    ui;
    ThreadDAQ      threadDaq;
    ThreadSaveFile threadSaveFile;
    ThreadShow     threadShow;

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
private slots:
    void startThread();
    void endThread();
    void slotNewValue(int diceValue);
};
#endif // DIALOG_H
