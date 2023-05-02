#include "TreeView.h"
#include "QDebug"
#include <QGridLayout>

VarTreeItem::VarTreeItem(const QVector<QVariant> &varDataVct, VarTreeItem *parent, bool isRootItem)
{
    parent = parentVarItem;
    mIsRootItem = isRootItem;
    mChecked = false;
    mEditable = false;
    setVarData(varDataVct);
}

const QString &VarTreeItem::getVarName() const
{
    return varName;
}

void VarTreeItem::setVarName(const QString &newVarName)
{
    varName = newVarName;
}

const QString &VarTreeItem::getVarValue() const
{
    return varValue;
}

void VarTreeItem::setVarValue(const QString &newVarValue)
{
    varValue = newVarValue;
}

const QString &VarTreeItem::getVarDes() const
{
    return varDes;
}

void VarTreeItem::setVarDes(const QString &newVarDes)
{
    varDes = newVarDes;
}

const QString &VarTreeItem::getVarUnit() const
{
    return varUnit;
}

void VarTreeItem::setVarUnit(const QString &newVarUnit)
{
    varUnit = newVarUnit;
}

const QString &VarTreeItem::getVarDisplayUnit() const
{
    return varDisplayUnit;
}

void VarTreeItem::setVarDisplayUnit(const QString &newVarDisplayUnit)
{
    varDisplayUnit = newVarDisplayUnit;
}

bool VarTreeItem::checked() const
{
    return mChecked;
}

void VarTreeItem::setChecked(bool newChecked)
{
    mChecked = newChecked;
}

bool VarTreeItem::editable() const
{
    return mEditable;
}

void VarTreeItem::setEditable(bool newEditable)
{
    mEditable = newEditable;
}


bool VarTreeItem::isRootItem() const
{
    return mIsRootItem;
}

void VarTreeItem::setIsRootItem(bool newIsRootItem)
{
    mIsRootItem = newIsRootItem;
}

VarTreeItem *VarTreeItem::parent() const
{
    return parentVarItem;
}

void VarTreeItem::setParent(VarTreeItem *newParent)
{
    parentVarItem = newParent;
}

VarTreeItem *VarTreeItem::childAt(int row)
{
    return mChildren.at(row);
}

void VarTreeItem::insertChild(int position, VarTreeItem *varTreeItem)
{
    mChildren.insert(position, varTreeItem);
}

void VarTreeItem::removeChild(VarTreeItem *varItem)
{
    mChildren.removeOne(varItem);
}

void VarTreeItem::removeChildren()
{
    qDeleteAll(mChildren);
    mChildren.clear();
}

int VarTreeItem::column() const
{
    return 5;
}

int VarTreeItem::row() const
{
    if(parentVarItem)
    {
        return parentVarItem->mChildren.indexOf(const_cast<VarTreeItem*>(this));
    }
    return 0;
}

bool VarTreeItem::setData(int column, const QVariant &value, int role)
{
    if(column == 0 && role == Qt::CheckStateRole)
    {
        if (value.toInt() == Qt::Checked) {
          setChecked(true);
        } else if (value.toInt() == Qt::Unchecked) {
          setChecked(false);
        }
        return true;
    }else if(column == 1 && role == Qt::EditRole){

    }
    return true;
}

QVariant VarTreeItem::data(int column, int role) const
{
    switch(column)
    {
    case 0:
    {
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::DecorationRole:
        case Qt::ToolTipRole:
            return varName;
        case Qt::FontRole:
        {
            QFont font;
            font.setBold(true);
            return font;
        }
        default:
          return QVariant();
        }
    }
    case 1:
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::DecorationRole:
        case Qt::ToolTipRole:
            return varValue;
        case Qt::FontRole:
        {
            if(isParameter())
            {
                QFont font;
                font.setBold(true);
                return font;
            }else
            {
               return QVariant();
            }

        }
        default:
          return QVariant();
        }
    case 2:
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return varUnit;
        default:
          return QVariant();
        }
    case 3:
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return varDisplayUnit;
        default:
          return QVariant();
        }
    case 4:
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return varDes;
        default:
          return QVariant();
        }
    default:
        return QVariant();
    }
}

void VarTreeItem::setVarData(const QVector<QVariant> &varDataVct)
{
    varName = varDataVct[VarTree::VarName].toString();
    varValue = varDataVct[VarTree::VarValue].toString();
    varDes = varDataVct[VarTree::VarDes].toString();
    varUnit = varDataVct[VarTree::VarUnit].toString();
    varDisplayUnit = varDataVct[VarTree::VarDisplayUnit].toString();
    varType = varDataVct[VarTree::VarType].toString();
    varDisplayName = varDataVct[VarTree::VarDisplayName].toString();
}

bool VarTreeItem::isParameter() const
{
   return varType.compare("parameter") == 0 || varType.compare("input") == 0;
}



VarTreeProxyModel::VarTreeProxyModel(QObject *parent):QSortFilterProxyModel(parent)
{

}

bool VarTreeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
     if (!filterRegExp().isEmpty())
     {
         QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
         if(index.isValid())
         {
             int rows = sourceModel()->rowCount(index);
             for (int i = 0 ; i < rows ; ++i)
             {
                 if (filterAcceptsRow(i, index))
                 {
                     return true;
                 }
             }
             VarTreeItem * varTreeItem = static_cast<VarTreeItem*>(index.internalPointer());
             if(varTreeItem)
             {
                 QString varName = varTreeItem->getVarName();
                 return varName.contains(filterRegExp());
             }
         }
     }
     return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

bool VarTreeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant l = (left.model() ? left.model()->data(left) : QVariant());
    QVariant r = (right.model() ? right.model()->data(right) : QVariant());
    return StringHandler::naturalSort(l.toString(), r.toString());
}

VarTreeModel::VarTreeModel(VarTreeView *varTreeView)
{
    setVarTreeView(varTreeView);
    QVector<QVariant> headers;
    headers << tr("变量") << tr("值")<< tr("显示单位") << tr("单位")
            << tr("注释") << tr("类型") << tr("显示变量");
    rootTreeItem = new VarTreeItem(headers, 0, true);
}

VarTreeView *VarTreeModel::getVarTreeView() const
{
    return varTreeView;
}

void VarTreeModel::setVarTreeView(VarTreeView *newVarTreeView)
{
    varTreeView = newVarTreeView;
}

VarTreeItem *VarTreeModel::getRootTreeItem() const
{
    return rootTreeItem;
}

void VarTreeModel::setRootTreeItem(VarTreeItem *newRootTreeItem)
{
    rootTreeItem = newRootTreeItem;
}

int VarTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
      return static_cast<VarTreeItem*>(parent.internalPointer())->column();
    else
        return rootTreeItem->column();
}

int VarTreeModel::rowCount(const QModelIndex &parent) const
{
    VarTreeItem * pParentVarTreeItem = nullptr;
    if (parent.column() > 0)
      return 0;

    if (!parent.isValid())
      pParentVarTreeItem = rootTreeItem;
    else
      pParentVarTreeItem = static_cast<VarTreeItem*>(parent.internalPointer());
    return pParentVarTreeItem->mChildren.size();
}

QVariant VarTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return rootTreeItem->data(section);
    return QVariant();
}

QModelIndex VarTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
      return QModelIndex();
    VarTreeItem * pParentVarTreeItem = nullptr;
    if (!parent.isValid())
      pParentVarTreeItem = rootTreeItem;
    else
      pParentVarTreeItem = static_cast<VarTreeItem*>(parent.internalPointer());
    VarTreeItem *pChildVarTreeItem = pParentVarTreeItem->childAt(row);
    if (pChildVarTreeItem)
      return createIndex(row, column, pChildVarTreeItem);
    else
        return QModelIndex();
}

QModelIndex VarTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
      return QModelIndex();
    VarTreeItem *pChildVarTreeItem = static_cast<VarTreeItem*>(index.internalPointer());
    VarTreeItem *pParentVarTreeItem = pChildVarTreeItem->parent();
    if (pParentVarTreeItem == rootTreeItem)
      return QModelIndex();
    return createIndex(pParentVarTreeItem->row(), 0, pParentVarTreeItem);
}

bool VarTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    VarTreeItem *pVarTreeItem = static_cast<VarTreeItem*>(index.internalPointer());
    if (!pVarTreeItem) {
      return false;
    }
    QString displayUnit = pVarTreeItem->getVarDisplayUnit();
    bool result = pVarTreeItem->setData(index.column(), value, role);
    if (index.column() == 0 && role == Qt::CheckStateRole) {

    } else if (index.column() == 1) { // value

    } else if (index.column() == 3) { // display unit

    }
//    updateVarTreeItem(pVarTreeItem);
    return result;
}

QVariant VarTreeModel::data(const QModelIndex &index, int role) const
{
    qDebug() << " retrun empty";
    if (!index.isValid())
    {
       qDebug() << " retrun empty";
       return QVariant();
    }


    VarTreeItem *pVarTreeItem = static_cast<VarTreeItem*>(index.internalPointer());
    return pVarTreeItem ->data(index.column(), role);
}

Qt::ItemFlags VarTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
      return Qt::ItemFlags();
    }
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    VarTreeItem *pVarTreeItem = static_cast<VarTreeItem*>(index.internalPointer());
    if (index.column() == 0){

    } else if (index.column() == 1 && pVarTreeItem && pVarTreeItem->mChildren.size() == 0 && pVarTreeItem->isParameter()) {
      flags |= Qt::ItemIsEditable;
    } else if (index.column() == 3) {
      flags |= Qt::ItemIsEditable;
    }
    return flags;
}

VarTreeItem *VarTreeModel::findVarTreeItem(const QString &name, VarTreeItem *pVarTreeItem, Qt::CaseSensitivity caseSensitivity) const
{
    if(pVarTreeItem->getVarName().compare(name,caseSensitivity) == 0)
    {
        return pVarTreeItem;
    }
    for(int i = pVarTreeItem->mChildren.size(); i>= 0; i--)
    {
        VarTreeItem * item = pVarTreeItem->childAt(i);
        if(findVarTreeItem(name,item))
        {
            return item;
        }
    }
    return nullptr;
}

VarTreeItem *VarTreeModel::findVarTreeItemOneLevel(const QString &name, VarTreeItem *pVarTreeItem, Qt::CaseSensitivity caseSensitivity) const
{
    if(!pVarTreeItem)
    {
        pVarTreeItem = rootTreeItem;
    }
    for(int i = pVarTreeItem->mChildren.size(); i>= 0; i--)
    {
        VarTreeItem * item = pVarTreeItem->childAt(i);
        if(item->getVarName().compare(name,caseSensitivity) == 0)
        {
            return item;
        }
    }
    return nullptr;
}

QModelIndex VarTreeModel::varTreeItemIndex(const VarTreeItem *pVarTreeItem) const
{
    return varTreeItemIndexHelper(pVarTreeItem, rootTreeItem, QModelIndex());
}

QModelIndex VarTreeModel::varTreeItemIndexHelper(const VarTreeItem *pVarTreeItem, const VarTreeItem *pParentVarTreeItem, const QModelIndex &parentIndex) const
{
    if(pVarTreeItem == rootTreeItem)
    {
        return parentIndex;
    }
    for(int i = pVarTreeItem->mChildren.size(); i>= 0; i--)
    {
        VarTreeItem * item = pVarTreeItem->mChildren.at(i);
        QModelIndex childIndex = index(i, 0, parentIndex);
        QModelIndex index = varTreeItemIndexHelper(pVarTreeItem,item, childIndex);
        if(index.isValid())
        {
            return index;
        }
    }
    return QModelIndex();
}

bool VarTreeModel::insertVarTreeItems(const QVector<QVariant> &varVct, VarTreeItem *parentItem)
{
    bool isRootItem = false;
    if(parentItem == nullptr)
    {
        parentItem = rootTreeItem;
    }
    if(parentItem == rootTreeItem)
    {
        isRootItem = true;
    }

//    QModelIndex index = varTreeItemIndex(parentItem);
//    int row = rowCount(index);
//    qDebug() << row;
    beginInsertRows(QModelIndex(), 0, 1);
    VarTreeItem * item = new VarTreeItem(varVct, rootTreeItem, isRootItem);
    rootTreeItem->insertChild(0,item);
    endInsertRows();
    return true;
}

VarTreeView::VarTreeView(VarTreeWidget *pVarWidget): QTreeView(pVarWidget)
{
    mpVarWidget = pVarWidget;

}

void VarTreeView::mouseReleaseEvent(QMouseEvent *event)
{

}

void VarTreeView::keyPressEvent(QKeyEvent *event)
{

}

VarTreeWidget::VarTreeWidget(QWidget *pParent):QWidget(pParent)
{
    varTreeView  = new VarTreeView(this);
    varTreeModel = new VarTreeModel(varTreeView);
    varTreeProxyModel = new VarTreeProxyModel;
    varTreeProxyModel->setDynamicSortFilter(true);
    varTreeProxyModel->setSourceModel(varTreeModel);
    varTreeView->setModel(varTreeProxyModel);
    varTreeView->setColumnWidth(0,150);
    varTreeView->setColumnWidth(1,70);
    varTreeView->setColumnWidth(2,50);
    varTreeView->setColumnWidth(3,70);
    varTreeView->setColumnHidden(2,true);

    QGridLayout * gridLayout = new QGridLayout;
    gridLayout->addWidget(varTreeView,0,0);
    this->setLayout(gridLayout);
}
