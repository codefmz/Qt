#ifndef WBATTERY_H
#define WBATTERY_H

#include <QColor>
#include <QWidget>

class QmyBattery : public QWidget
{
    Q_OBJECT
    //自定义属性
    Q_PROPERTY(int powerLevel READ powerLevel WRITE setPowerLevel NOTIFY powerLevelChanged)

private:
    QColor mColorBack = Qt::white;   //背景颜色
    QColor mColorBorder = Qt::black; //电池边框颜色
    QColor mColorPower = Qt::green;  //电量柱颜色
    QColor mColorWarning = Qt::red;  //电量短缺时的颜色
    QColor mColorPass = Qt::yellow; //黄色
    int mPowerLevel = 60; //电量0-100
    int mWarnLevel = 20;  //电量低警示阈值
    int mPassLevel = 40;  //电量合格阈值
protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE; //报告缺省大小
public:
    explicit QmyBattery(QWidget* parent = 0);
    void setPowerLevel(int pow); //设置当前电量
    int  powerLevel();
    void setWarnLevel(int warn); //设置电量低阈值
    int  warnLevel();

signals:
    void powerLevelChanged(int);

public slots:
};

#endif // WBATTERY_H
