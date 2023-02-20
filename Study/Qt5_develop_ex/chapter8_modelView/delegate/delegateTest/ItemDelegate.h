#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H
#include <QItemDelegate>
#include <QTextDocument>
class ItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ItemDelegate(QObject *pParent = 0, bool drawRichText = false, bool drawGrid = false);
    QColor getGridColor() {return mGridColor;}
    void setGridColor(QColor color) {mGridColor = color;}
    QString formatDisplayText(QVariant variant) const;
    void initTextDocument(QTextDocument *pTextDocument, QFont font, int width) const;
    void drawHover(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    virtual QWidget* createEditor(QWidget *pParent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  public slots:
    void unitComboBoxChanged(QString text);
private:
    bool mDrawRichText;
    QPoint mLastTextPos;
    bool mDrawGrid;
    QColor mGridColor;
    QObject *mpParent;
};

#endif // ITEMDELEGATE_H
