#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    //基本绘图常识
    void paint1();
    //视口与窗口
    void paint2();
signals:

private:
    QPainterPath getPainterPath(int state,  qreal deg = 0,  qreal R = 100 );
};

#endif // MYWIDGET_H
