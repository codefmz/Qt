#include "SpinboxDelegate.h"
#include "QSpinBox"
SpinboxDelegate::SpinboxDelegate(QObject *parent)
    : QItemDelegate{parent}
{

}

QWidget *SpinboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QSpinBox * spinBox = new QSpinBox(parent);
    spinBox->setRange(0,1000000000);
    spinBox->setSingleStep(1000);
    spinBox->installEventFilter(const_cast<SpinboxDelegate*>(this));
    return spinBox;
}

void SpinboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   int str = index.model()->data(index).toInt();
   QSpinBox * spinBox = static_cast<QSpinBox*>(editor);
   spinBox->setValue(str);
}

void SpinboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox * spinBox = static_cast<QSpinBox*>(editor);
    int value = spinBox->value();
    model->setData(index,value);
}

void SpinboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
