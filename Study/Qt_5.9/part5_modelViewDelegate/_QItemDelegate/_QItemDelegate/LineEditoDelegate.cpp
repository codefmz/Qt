#include "LineEditoDelegate.h"
#include <QLineEdit>
#include <QPainterPath>
LineEditoDelegate::LineEditoDelegate(QObject* parent)
    : QItemDelegate { parent }
{
}

QWidget* LineEditoDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return new QLineEdit(parent);
}

void LineEditoDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString    dataStr = index.data().toString();
    QLineEdit* edit = qobject_cast<QLineEdit*>(editor);
    edit->setText(dataStr);
}

void LineEditoDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(editor);
    QString    newData = edit->text();
    model->setData(index, QVariant(newData));
}

void LineEditoDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void LineEditoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem myOpt = option;
    QPalette myPalette;
    myPalette.setBrush(QPalette::Highlight, Qt::darkGreen);
    myOpt.palette = myPalette;

    QPainterPath path;
    int x1, y1, x2, y2;
    myOpt.rect.getCoords(&x1, &y1, &x2, &y2);
    path.addRect(x1 + 1,y1 + 1, myOpt.rect.width() - 2, myOpt.rect.height() - 2);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen = painter->pen();
    pen.setWidthF(1);
    painter->setPen(pen);
    painter->drawPath(path);
    painter->restore();
    QItemDelegate::paint(painter,myOpt,index);
}

//QSize LineEditoDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    return QItemDelegate::sizeHint(option,index);
//}
