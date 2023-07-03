#ifndef EDITOR_H
#define EDITOR_H
#include <QCompleter>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QStandardItemModel>
#include <QSyntaxHighlighter>
class LineNumberArea;
class TextBlockUserData;
class DocumentMarker;
class BreakpointMarker;
class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    PlainTextEdit(QWidget* parent = nullptr);

    void setCompletionCharacters(QString chars)
    {
        mCompletionCharacters = chars;
    }
    void setCanHaveBreakpoints(bool canHaveBreakpoints);
    bool canHaveBreakpoints()
    {
        return mCanHaveBreakpoints;
    }
    DocumentMarker* getDocumentMarker()
    {
        return mpDocumentMarker;
    }
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
    static int         foldBoxWidth(const QFontMetrics& fm);

private:
    void    initCompleteModel();
    QString wordUnderCursor();
    void    indentOrUnindent(bool doIndent);
    void    toggleBreakpoint(const QString fileName, int lineNumber);
    void    toggleBlockVisible(const QTextBlock& block);
    void    moveCursorVisible(bool ensureVisible);

protected:
    virtual void resizeEvent(QResizeEvent* pEvent) override;
    virtual void keyPressEvent(QKeyEvent* pEvent) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
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

    void slotUndoAvailable();

private:
    LineNumberArea*              mpLineNumberArea;
    bool                         mCanHaveBreakpoints;
    QTextCharFormat              mParenthesesMatchFormat;
    QTextCharFormat              mParenthesesMisMatchFormat;
    QStandardItemModel*          mpStandardItemModel;
    QCompleter*                  mpCompleter;
    QString                      mCompletionCharacters;
    DocumentMarker*              mpDocumentMarker;
    QMap<int, BreakpointMarker*> lineDocMap;
    static int                   mTabSize;
    static int                   mIndentSize;
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

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    QSize        sizeHint() const override;
    virtual void mouseMoveEvent(QMouseEvent* event) override
    {
        mpEidtor->lineNumberAreaMouseEvent(event);
    }
    virtual void mousePressEvent(QMouseEvent* event) override
    {
        mpEidtor->lineNumberAreaMouseEvent(event);
    }

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

struct Parenthesis
{
    enum Type
    {
        Opened,
        Closed
    };
    inline Parenthesis()
        : type(Opened)
        , pos(-1)
    {
    }
    inline Parenthesis(Type t, QChar c, int position)
        : type(t)
        , chr(c)
        , pos(position)
    {
    }
    Type  type;
    QChar chr;
    int   pos;
};
typedef QVector<Parenthesis> Parentheses;

class BaseEditorDocumentLayout : public QPlainTextDocumentLayout
{
    Q_OBJECT
public:
    BaseEditorDocumentLayout(QTextDocument* document);
    static Parentheses        parentheses(const QTextBlock& block);
    static bool               hasParentheses(const QTextBlock& block);
    static void               setFoldingIndent(const QTextBlock& block, int indent);
    static int                foldingIndent(const QTextBlock& block);
    static bool               canFold(const QTextBlock& block);
    static void               foldOrUnfold(const QTextBlock& block, bool unfold);
    static bool               isFolded(const QTextBlock& block);
    static void               setFolded(const QTextBlock& block, bool folded);
    static TextBlockUserData* testUserData(const QTextBlock& block);
    static TextBlockUserData* userData(const QTextBlock& block);
    void                      emitDocumentSizeChanged()
    {
        emit documentSizeChanged(documentSize());
    }
    bool mHasBreakpoint;
};

class ITextMark : public QObject
{
    Q_OBJECT
public:
    ITextMark(QObject* parent = 0)
        : QObject(parent)
    {
    }
    virtual ~ITextMark() {}

    virtual QIcon icon() const = 0;
    virtual void  updateLineNumber(int lineNumber) = 0;
    virtual void  updateBlock(const QTextBlock& block) = 0;
    virtual void  removeFromEditor() = 0;
    virtual void  documentClosing() = 0;
};

typedef QList<ITextMark*> TextMarks;

class ITextMarkable : public QObject
{
    Q_OBJECT
public:
    ITextMarkable(QObject* parent = 0)
        : QObject(parent)
    {
    }
    virtual ~ITextMarkable() {}

    virtual bool      addMark(ITextMark* mark, int line) = 0;
    virtual TextMarks marksAt(int line) const = 0;
    virtual void      removeMark(ITextMark* mark) = 0;
    virtual bool      hasMark(ITextMark* mark) const = 0;
    virtual void      updateMark(ITextMark* mark) = 0;
};

class BreakpointMarker : public ITextMark
{
    Q_OBJECT
public:
    BreakpointMarker(const QString& fileName, int lineNumber);

    // ITextMark
    // returns Breakpoint icon
    virtual QIcon icon() const override;
    // called if the lineNumber changes
    virtual void updateLineNumber(int lineNumber) override;
    // called whenever the text of the block for the marker changed
    virtual void updateBlock(const QTextBlock& block) override;
    // called if the block containing this mark has been removed
    // if this also removes your mark call this->deleteLater();
    virtual void removeFromEditor() override;
    virtual void documentClosing() override;

    void setFilePath(QString filePath)
    {
        mpFileName = filePath;
    }
    inline QString filePath() const
    {
        return mpFileName;
    }
    inline QString fileName() const
    {
        return QFileInfo(mpFileName).fileName();
    }
    inline QString path() const
    {
        return QFileInfo(mpFileName).path();
    }
    inline QString lineText() const
    {
        return mpLineText;
    }
    void setLineNumber(int lineNumber)
    {
        mpLineNumber = lineNumber;
    }
    inline int lineNumber() const
    {
        return mpLineNumber;
    }
    void setEnabled(bool enable)
    {
        mEnabled = enable;
    }
    inline bool isEnabled() const
    {
        return mEnabled;
    }
    void setIgnoreCount(int ignoreCount)
    {
        mIgnoreCount = ignoreCount;
    }
    inline int getIgnoreCount()
    {
        return mIgnoreCount;
    }
    void setCondition(QString condition)
    {
        mCondition = condition;
    }
    inline QString getCondition()
    {
        return mCondition;
    }

private:
    //    BreakpointsTreeModel* mpBreakpointsTreeModel;
    QString mpFileName;
    int     mpLineNumber;
    QString mpLineText;
    bool    mEnabled;
    int     mIgnoreCount;
    QString mCondition;
};

class DocumentMarker : public ITextMarkable
{
    Q_OBJECT
public:
    DocumentMarker(QTextDocument*, int = 0);

    // ITextMarkable
    bool      addMark(ITextMark* mark, int line);
    TextMarks marksAt(int line) const;
    void      removeMark(ITextMark* mark);
    bool      hasMark(ITextMark* mark) const;
    void      updateMark(ITextMark* mark);

    void updateBreakpointsLineNumber();
    void updateBreakpointsBlock(const QTextBlock& block);

private:
    QTextDocument* mpTextDocument;
    int            mLineStartNumber;
};

class TextBlockUserData : public QTextBlockUserData
{
public:
    inline TextBlockUserData()
        : mFoldingIndent(0)
        , mFolded(false)
        , mFoldingEndIncluded(false)
        , mFoldingState(false)
        , mFoldingEndState(false)
        , mFoldingEnd(false)
        , mFoldingStartIndex(-1)
        , mLeadingSpaces(-1)
    {
    }
    ~TextBlockUserData();

    inline TextMarks marks() const
    {
        return _marks;
    }
    inline void addMark(ITextMark* mark)
    {
        _marks += mark;
    }
    inline bool removeMark(ITextMark* mark)
    {
        return _marks.removeAll(mark);
    }
    inline bool hasMark(ITextMark* mark) const
    {
        return _marks.contains(mark);
    }
    inline void clearMarks()
    {
        _marks.clear();
    }
    inline void documentClosing()
    {
        foreach (ITextMark* tm, _marks)
        {
            tm->documentClosing();
        }
        _marks.clear();
    }

    void setParentheses(const Parentheses& parentheses)
    {
        mParentheses = parentheses;
    }
    Parentheses parentheses()
    {
        return mParentheses;
    }
    inline void clearParentheses()
    {
        mParentheses.clear();
    }
    inline bool hasParentheses() const
    {
        return !mParentheses.isEmpty();
    }
    enum MatchType
    {
        NoMatch,
        Match,
        Mismatch
    };
    static MatchType checkOpenParenthesis(QTextCursor* cursor, QChar c);
    static MatchType checkClosedParenthesis(QTextCursor* cursor, QChar c);
    static MatchType matchCursorBackward(QTextCursor* cursor);
    static MatchType matchCursorForward(QTextCursor* cursor);

    /* Set the code folding level.
     * A code folding marker will appear the line *before* the one where the indention
     * level increases. The code folding region will end in the last line that has the same
     * indention level (or higher).
     */
    inline int foldingIndent() const
    {
        return mFoldingIndent;
    }
    inline void setFoldingIndent(int indent)
    {
        mFoldingIndent = indent;
    }
    inline void setFolded(bool b)
    {
        mFolded = b;
    }
    inline bool folded() const
    {
        return mFolded;
    }
    // Set whether the last character of the folded region will show when the code is folded.
    inline void setFoldingEndIncluded(bool foldingEndIncluded)
    {
        mFoldingEndIncluded = foldingEndIncluded;
    }
    inline bool foldingEndIncluded() const
    {
        return mFoldingEndIncluded;
    }
    inline void setFoldingState(bool foldingState)
    {
        mFoldingState = foldingState;
    }
    inline bool foldingState() const
    {
        return mFoldingState;
    }
    inline void setFoldingEndState(bool foldingEndState)
    {
        mFoldingEndState = foldingEndState;
    }
    inline bool foldingEndState() const
    {
        return mFoldingEndState;
    }
    inline void setFoldingEnd(bool foldingEnd)
    {
        mFoldingEnd = foldingEnd;
    }
    inline bool foldingEnd() const
    {
        return mFoldingEnd;
    }
    inline void setFoldingStartIndex(int foldingStartIndex)
    {
        mFoldingStartIndex = foldingStartIndex;
    }
    inline int foldingStartIndex() const
    {
        return mFoldingStartIndex;
    }

    inline void setLeadingSpaces(int leadingSpaces)
    {
        mLeadingSpaces = leadingSpaces;
    }
    inline int getLeadingSpaces()
    {
        return mLeadingSpaces;
    }

private:
    TextMarks   _marks;
    Parentheses mParentheses;
    int         mFoldingIndent;
    bool        mFolded;
    bool        mFoldingEndIncluded;
    bool        mFoldingState;
    bool        mFoldingEndState;
    bool        mFoldingEnd;
    int         mFoldingStartIndex;
    int         mLeadingSpaces;
};

#endif // EDITOR_H
