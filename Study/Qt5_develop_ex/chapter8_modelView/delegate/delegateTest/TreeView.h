#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>
#include <QSortFilterProxyModel>
#include "StringHandler.h"

#if _MSC_VER >= 1600    //解决msvc中文乱码的问题
#pragma execution_character_set("utf-8")
#endif
namespace VarTree{

enum VarType{
    VarName = 0,
    VarValue,
    VarUnit,
    VarDisplayUnit,
    VarDes,
    VarType,
    VarDisplayName
};
}


class VarTreeItem
{
public:
    VarTreeItem(const QVector<QVariant> & varDataVct, VarTreeItem * parent = 0, bool isRootItem = false);
    bool isParameter() const;
    /*---------get and set-----------*/
    const QString &getVarName() const;
    void setVarName(const QString &newVarName);
    const QString &getVarValue() const;
    void setVarValue(const QString &newVarValue);
    const QString &getVarDes() const;
    void setVarDes(const QString &newVarDes);
    const QString &getVarUnit() const;
    void setVarUnit(const QString &newVarUnit);
    const QString &getVarDisplayUnit() const;
    void setVarDisplayUnit(const QString &newVarDisplayUnit);
    bool checked() const;
    void setChecked(bool newChecked);
    bool editable() const;
    void setEditable(bool newEditable);
    const QString &variability() const;
    void setVariability(const QString &newVariability);
    bool isRootItem() const;
    void setIsRootItem(bool newIsRootItem);

    /*------ parent and child data----------*/
    VarTreeItem *parent() const;
    void setParent(VarTreeItem *newParent);
    VarTreeItem * childAt(int row);
    void insertChild(int position, VarTreeItem * varTreeItem);
    void removeChild(VarTreeItem * varItem);
    void removeChildren();
    int column() const;
    int row() const;
    bool setData(int column, const QVariant &value, int role = Qt::EditRole);
    QVariant data(int column, int role = Qt::DisplayRole) const;

private:
    void setVarData(const QVector<QVariant> & varDataVct);


private:
    VarTreeItem * parentVarItem;
    QString varName;
    QString varValue;
    QString varDes;
    QString varUnit;
    QString varDisplayUnit;
    bool mChecked;
    bool mEditable;
    bool mIsRootItem;
    QString varType;
    QString varDisplayName;
public:
    QList<VarTreeItem*> mChildren;
};

class VarTreeWidget;
class VarTreeView : public QTreeView
{
  Q_OBJECT
public:
  VarTreeView(VarTreeWidget *pVarWidget);
  VarTreeWidget * mpVarWidget;
protected:
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void keyPressEvent(QKeyEvent *event) override;
};

class VarTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    VarTreeProxyModel(QObject *parent = 0);
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};


class VarTreeModel : public QAbstractItemModel
{
  Q_OBJECT
public:
    VarTreeModel(VarTreeView * varTreeView);

    /******* get and set*******/
    VarTreeView *getVarTreeView() const;
    void setVarTreeView(VarTreeView *newVarTreeView);
    VarTreeItem *getRootTreeItem() const;
    void setRootTreeItem(VarTreeItem *newRootTreeItem);

    /****** model overide function ******/
    int columnCount(const QModelIndex & parent = QModelIndex())const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex & index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /********* manage item *******/
    VarTreeItem *findVarTreeItem(const QString &name, VarTreeItem *pVarTreeItem, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive) const;
    VarTreeItem* findVarTreeItemOneLevel(const QString &name, VarTreeItem *pVarTreeItem = 0, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive) const;
    QModelIndex  varTreeItemIndex(const VarTreeItem *pVarTreeItem) const;
    QModelIndex  varTreeItemIndexHelper(const VarTreeItem *pVarTreeItem, const VarTreeItem *pParentVarTreeItem, const QModelIndex &parentIndex) const;
    bool insertVarTreeItems(const QVector<QVariant> & varVct, VarTreeItem * parentItem = nullptr);


private:
    VarTreeView * varTreeView;
    VarTreeItem * rootTreeItem;
};

class VarTreeWidget : public QWidget
{
    Q_OBJECT
public:
    VarTreeWidget(QWidget *pParent = 0);

public:
    VarTreeProxyModel * varTreeProxyModel;
    VarTreeModel * varTreeModel;
    VarTreeView * varTreeView;

};
#endif // TREEVIEW_H
