#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include<QGraphicsView>
#include <QWidget>
#include <QMouseEvent>
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
//    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void keyPress(QKeyEvent * event);
    void mouseDoubleClicked(QPoint point);
    void mouseClicked(QPoint point);
//    void mouseMove(QPoint )


};

#endif // GRAPHICSVIEW_H
