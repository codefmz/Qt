#include "DateDelegate.h"
#include "QApplication"
#include "QDateTimeEdit"
#include "QDebug"
DateDelegate::DateDelegate(QObject* parent)
    : QItemDelegate { parent }
{
}

QWidget* DateDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    qDebug() << "DateDelegate::createEditor";
    QDateTimeEdit* editor = new QDateTimeEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd");
    editor->setCalendarPopup(true); //设置日历选择以 Popup 的方法，即下拉菜的方式显示
    //调用 QObject 类的 installEventFilter() 函数安装时间过滤器， 使得 DateDelegate 能够捕获 QDateTimeData 对象的事件
    editor->installEventFilter(const_cast<DateDelegate*>(this));
    //    connect(editor,&QDateTimeEdit::editingFinished, this,&DateDelegate::commitAndCloseEditor);
    return editor;
}

void DateDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    qDebug() << "DateDelegate::setEditorData";
    // index.model  获取 index 对应的 model 对象， data() 返回一个 QVariant 对象
    QString        dateStr = index.model()->data(index).toString();
    QDate          date = QDate::fromString(dateStr, Qt::ISODate);
    QDateTimeEdit* edit = static_cast<QDateTimeEdit*>(editor);
    edit->setDate(date);
}

void DateDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    qDebug() << "DateDelegate::setModelData";
    QDateTimeEdit* edit = static_cast<QDateTimeEdit*>(editor);
    QDate          date = edit->date();
    model->setData(index, QVariant(date.toString(Qt::ISODate)));
}

void DateDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    qDebug() << "DateDelegate::updateEditorGeometry";
    editor->setGeometry(option.rect);
}

void DateDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    qDebug() << "DateDelegate::paint";
//    QString        dateStr = index.model()->data(index).toString();
//    QStyleOptionButton btnOpt;
//    btnOpt.features = QStyleOptionButton::AutoDefaultButton;
//    btnOpt.rect = option.rect;
//    btnOpt.palette = QPalette(Qt::cyan);
//    btnOpt.text = dateStr;

    QPainterPath roundRect = roundRectPath(option.rect);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->setPen(Qt::cyan);
    QPen pen = painter->pen();
    pen.setWidthF(1.5);
    painter->setPen(pen);
    painter->drawPath(roundRect);
    painter->restore();


//    QApplication::style()->drawControl(QStyle::CE_PushButton,&btnOpt,painter);

    QItemDelegate::paint(painter, option, index);
}

QSize DateDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option,index);
}

void DateDelegate::commitAndCloseEditor()
{
    QDateTimeEdit* editor = qobject_cast<QDateTimeEdit*>(sender());
    emit           commitData(editor);
    emit           closeEditor(editor);
}

QPainterPath DateDelegate::roundRectPath(const QRect &rect) const
{
    int x1, y1, x2, y2;
    int radius = qMin(rect.width(), rect.height()) / 2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    int dbase = radius / 4;
    int yc = y1 + radius;
    int xc = x2 - 3 * dbase;
    QPainterPath path;
    path.moveTo(xc +  dbase, yc);
    path.lineTo(xc, yc - dbase);
    path.lineTo(xc - dbase, yc);
    return path;
}
