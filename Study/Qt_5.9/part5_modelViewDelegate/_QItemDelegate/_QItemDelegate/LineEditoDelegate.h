#ifndef LINEEDITODELEGATE_H
#define LINEEDITODELEGATE_H

#include <QItemDelegate>
#include <QPainter>
class LineEditoDelegate : public QItemDelegate
{
public:
    explicit LineEditoDelegate(QObject *parent = nullptr);

    //创建控件， 由 QModelIndex 对象指定的表项数据的编辑控件，并对控件的内容进行限定
    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    //设置控件显示的数据，并将Model 中的数据跟你更新到 Delegate 中，相当于一个初始化工作
    void setEditorData(QWidget * editor, const QModelIndex & index) const override;
    //将 Delegate 中对数据的改变更新至 Model 中
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override;
    //更新控件的几何位置
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    // 绘制item显示的
    void paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //设置容器的位置与大小
//    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;


};

#endif // LINEEDITODELEGATE_H
