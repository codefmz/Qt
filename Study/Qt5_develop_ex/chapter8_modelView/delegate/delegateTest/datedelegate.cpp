#include "DateDelegate.h"
#include "QDateTimeEdit"
#include "QDebug"
#include "QApplication"
DateDelegate::DateDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << "DateDelegate::createEditor";
    QDateTimeEdit * editor = new QDateTimeEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd");
    editor->setCalendarPopup(true); //设置日历选择以 Popup 的方法，即下拉菜的方式显示
    //调用 QObject 类的 installEventFilter() 函数安装时间过滤器， 使得 DateDelegate 能够捕获 QDateTimeData 对象的事件
    editor->installEventFilter(const_cast<DateDelegate*>(this));
    return editor;
}

void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug() << "DateDelegate::setEditorData";
    //index.model  获取 index 对应的 model 对象， data() 返回一个 QVariant 对象
    QString dateStr = index.model()->data(index).toString();
    QDate date = QDate::fromString(dateStr, Qt::ISODate);
    QDateTimeEdit * edit = static_cast<QDateTimeEdit*>(editor);
    edit->setDate(date);
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug() << "DateDelegate::setModelData";
    QDateTimeEdit * edit = static_cast<QDateTimeEdit*>(editor);
    QDate date = edit->date();
    model->setData(index, QVariant(date.toString(Qt::ISODate)));
}

void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << "DateDelegate::updateEditorGeometry";
    editor->setGeometry(option.rect);
}

void DateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << "DateDelegate::paint";
    QStyleOption comBoxOption;
    comBoxOption.initFrom(option.widget);
    painter->save();

    QPen pen;
    pen.setColor(Qt::black);
//    for (int i = 1; i < 5; ++i)
//    {
//        starPolygon << 10 * QPointF(0.5 + 0.5 * std::cos(0.8 * i * 3.14),
//                                    0.5 + 0.5 * std::sin(0.8 * i * 3.14));
//    }
    pen.setWidth(2);
    painter->setPen(pen);
    QVector<QPointF> vct(3);
    vct << QPointF(0,0) << QPointF(6, 10) << QPointF(20,0);
    QRect rt = option.rect;
    painter->translate(rt.right() - 20, rt.center().ry());
    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->setPen(Qt::SolidLine);
    painter->drawLine(QPointF(0,0), QPointF(6, 6));
    painter->drawLine(QPointF(6,6), QPointF(12, 0));
    painter->restore();

//    QString dateStr = index.model()->data(index).toString();
//    comBoxOption.text = dateStr;
//    comBoxOption.rect = option.rect;

//    QApplication::style()->drawControl(QStyle::CE_PushButton, &comBoxOption, painter);

    QItemDelegate::paint(painter, option, index);

}
