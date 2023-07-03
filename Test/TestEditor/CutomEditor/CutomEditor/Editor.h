#ifndef EDITOR_H
#define EDITOR_H
#include <QCompleter>
#include <QPlainTextEdit>
#include <QStandardItemModel>

class LineNumberArea;
class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    PlainTextEdit(QWidget* parent = nullptr);
    int             lineNumberAreaWidth();
    void            lineNumberAreaPaintEvent(QPaintEvent* event);
    void            lineNumberAreaMouseEvent(QMouseEvent* event);
    LineNumberArea* getMpLineNumberArea() const;
    void            insertCompleterTypes(QStringList types);

    // 工具 util 函数，占时没有放到
    static QStringList getKeywords();
    static QStringList getTypes();
    static int         firstNonSpace(const QString& text);

private:
    void    initCompleteModel();
    QString wordUnderCursor();
    void resetZoom();
    void zoomIn();
    void zoomOut();
protected:
    virtual void resizeEvent(QResizeEvent* pEvent) override;
    virtual void keyPressEvent(QKeyEvent* pEvent) override;
    virtual void wheelEvent(QWheelEvent* event) override;
public slots:
    /**
     * @brief:根据文本的行数，更新PlainEdit左边距
     */
    void updateLineNumberAreaWidth(int newBlockCount);
    /**
     * @brief: 矩形文本内容变化更新 lineNumArea， lineNumArea跟随滚动
     */
    void updateLineNumberArea(const QRect& rect, int dy);
private slots:
    /**
     * @brief: 显示自动补全概述窗口
     */
    void showCompletionItemToolTip(const QModelIndex& index);
    /**
     * @file brief: 自动补全插入
     */
    void insertCompletionItem(const QModelIndex& index);

private:
    LineNumberArea*     mpLineNumberArea;
    QStandardItemModel* mpStandardItemModel;
    QCompleter*         mpCompleter;
};

/**
 * @class brief: 自动补全节点数据
 */
class CompleterItem
{
public:
    CompleterItem() {}
    CompleterItem(const QString& key, const QString& value, const QString& select);
    CompleterItem(const QString& key, const QString& value, const QString& select, const QString& description);
    CompleterItem(const QString& value, const QString& description);
    QString mKey;
    QString mValue;
    QString mSelect;
    QString mDescription;
    //排序与查找实现自定义比较规则
    bool operator<(const CompleterItem& other) const
    {
        return (mKey < other.mKey) || ((mKey == other.mKey) && (mDescription < other.mDescription));
    }
    bool operator==(const CompleterItem& other) const
    {
        return mKey == other.mKey && mDescription == other.mDescription;
    }
};
Q_DECLARE_METATYPE(CompleterItem) //通知QMetaType知道这个类型， 使用模板函数需要用到，主要是 QVarint

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QWidget* eidtor);
    QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    PlainTextEdit* mpEidtor;
};

#endif // EDITOR_H
