#include "FixedCheckBox.h"
#include <QStylePainter>
#include <QStyleOption>
FixedCheckBox::FixedCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    setCheckable(false);
    mDefaultValue = false;
    mInheritedValue = false;
    mFixedState = false;
}

void FixedCheckBox::setTickState(bool defaultValue, bool fixedState)
{
    mDefaultValue = defaultValue;
    if (mDefaultValue)
    {
        mInheritedValue = fixedState;
    }
    else
    {
        mFixedState = fixedState;
    }
}

QString FixedCheckBox::getTickStateString() const
{
    if (mDefaultValue)
    {
        return "";
    }
    else if (mFixedState)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

void FixedCheckBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStylePainter      p(this);
    QStyleOptionButton opt;
    opt.initFrom(this);
    if (mDefaultValue)
    {
        p.setBrush(QColor(225, 225, 225));
    }
    else
    {
        p.setBrush(Qt::white);
    }
    p.drawRect(opt.rect.adjusted(0, 0, -1, -1));
    // if is checked then draw a tick
    if ((!mDefaultValue && mFixedState) || (mDefaultValue && mInheritedValue))
    {
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen = p.pen();
        pen.setWidthF(1.5);
        p.setPen(pen);
        QVector<QPoint> lines;
        lines << QPoint(opt.rect.left() + 3, opt.rect.center().y());
        lines << QPoint(opt.rect.center().x() - 1, opt.rect.bottom() - 3);
        lines << QPoint(opt.rect.center().x() - 1, opt.rect.bottom() - 3);
        lines << QPoint(opt.rect.width() - 3, opt.rect.top() + 3);
        p.drawLines(lines);
    }
}
