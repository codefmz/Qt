#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QSharedMemory>
#include <QThread>
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget* parent = nullptr);
    void setUpWindow();

private:
    void init();
    int  startProcess(const QString& exeFilePath);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QLabel*       commandLabel;
    QLineEdit*    commandLineEdit;
    QPushButton*  sendBtn;
    QLabel*       picLabel;
    QSharedMemory sharedMemoryA;
    QString       keyA2B;
    QSharedMemory sharedMemoryB;
    QString       keyB2A;
    QString       exeFilePath;
    QProcess*     process;
    bool          isProcessRunning;
private slots:
    void slotSend();
    void slotProcessStarted();
    void readProcessStandardOutput();
    void readProcessStandardError();
    void processError(QProcess::ProcessError);
    void processFinished(int, QProcess::ExitStatus);
};

#endif // WIDGET_H
