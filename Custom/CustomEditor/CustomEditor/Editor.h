#ifndef EDITOR_H
#define EDITOR_H
#include <QCompleter>
#include <QPlainTextEdit>
#include <QStandardItemModel>
#include <QSyntaxHighlighter>
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

    // util 函数， copy 过来的，没仔细看
    static QStringList getKeywords();
    static QStringList getTypes();
    static int         firstNonSpace(const QString& text);
    static int         lineIndentPosition(const QString& text);
    static int         spacesLeftFromPosition(const QString& text, int position);
    static int         columnAt(const QString& text, int position);
    static int         indentedColumn(int column, bool doIndent = true);
    static QString     indentationString(int startColumn, int targetColumn);
    static bool        cursorIsAtBeginningOfLine(const QTextCursor& cursor);

private:
    void    initCompleteModel();
    QString wordUnderCursor();
    void    indentOrUnindent(bool doIndent);

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

    void resetZoom();
    void zoomIn();
    void zoomOut();
private slots:
    /**
     * @brief: 显示自动补全概述窗口
     */
    void showCompletionItemToolTip(const QModelIndex& index);
    /**
     * @brief: 自动补全插入
     */
    void insertCompletionItem(const QModelIndex& index);

    /**
     * @brief 选中行高亮
     */
    void updateHighlights();
    void updateCursorPosition();
private:
    LineNumberArea*     mpLineNumberArea;
    QStandardItemModel* mpStandardItemModel;
    QCompleter*         mpCompleter;
    static int          mTabSize;
    static int          mIndentSize;
};

/**
 * @class : 自动补全节点数据
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

/**
 *@class : 行号窗口
 */
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QWidget* eidtor);
//    QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    PlainTextEdit* mpEidtor;
};

/**
 *@class : 语法着色类，必须实现 highlightBlock 函数
 */
class TextHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    TextHighlighter(QPlainTextEdit* pPlainTextEdit = 0);
    void initializeSettings();

    /**
     * @brief: 高亮多行，用来处理单行或者多行注释
     */
    void highlightMultiLine(const QString& text);

protected:
    //自动调用，必须是西安
    virtual void highlightBlock(const QString& text) override;

private:
    QPlainTextEdit* mpPlainTextEdit;
    struct HighlightingRule
    {
        QRegExp         mPattern;
        QTextCharFormat mFormat;
    };
    QVector<HighlightingRule> mHighlightingRules;
    QTextCharFormat           mTextFormat;
    QTextCharFormat           mKeywordFormat;
    QTextCharFormat           mTypeFormat;
    QTextCharFormat           mFunctionFormat;
    QTextCharFormat           mQuotationFormat;
    QTextCharFormat           mSingleLineCommentFormat;
    QTextCharFormat           mMultiLineCommentFormat;
    QTextCharFormat           mNumberFormat;
};

#endif // EDITOR_H
