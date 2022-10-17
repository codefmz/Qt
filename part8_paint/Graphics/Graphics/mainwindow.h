#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initGraphics();

protected:
    void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
private:
    Ui::MainWindow *ui;
//    GraphicsView * graphicsView;
    QLabel * labViewCord;
    QLabel * labSceneCord;
    QLabel * labItemCord;

    QGraphicsScene * graphScene;

private slots:
    void on_mouseMove(QPoint point);
    void on_mouseClicked(QPoint point);

};
#endif // MAINWINDOW_H
