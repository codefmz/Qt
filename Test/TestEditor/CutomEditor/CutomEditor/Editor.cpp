#include "Editor.h"
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QTextBlock>
#include <QAbstractItemView>
#include <QScrollBar>
PlainTextEdit::PlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent)
{
    setStyleSheet("QPlainTextEdit{"
                  "selection-background-color:#3399FF;"
                  "selection-color: white;"
                  "border: 1px solid gray;}");

    QTextDocument* pTextDocument = document();
    //文本距离上下左右的边距
    pTextDocument->setDocumentMargin(2);
    setDocument(pTextDocument);
    //功能1：数字行号
    mpLineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    updateLineNumberAreaWidth(0);
    setLineWrapMode(QPlainTextEdit::WidgetWidth); //设置换行模式

    //功能2：自动补全
    mpStandardItemModel = new QStandardItemModel(this);
    initCompleteModel();
    QSortFilterProxyModel* pSortFilterProxyModel = new QSortFilterProxyModel(this);
    pSortFilterProxyModel->setSourceModel(mpStandardItemModel);
    pSortFilterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    pSortFilterProxyModel->sort(0, Qt::AscendingOrder);
    mpCompleter = new QCompleter(this);
    mpCompleter->setModel(pSortFilterProxyModel);
    mpCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mpCompleter->setWrapAround(false);
    mpCompleter->setWidget(this);
    mpCompleter->setCompletionMode(QCompleter::PopupCompletion);
    connect(mpCompleter, SIGNAL(highlighted(QModelIndex)), this, SLOT(showCompletionItemToolTip(QModelIndex)));
    connect(mpCompleter, SIGNAL(activated(QModelIndex)), this, SLOT(insertCompletionItem(QModelIndex)));
}

int PlainTextEdit::lineNumberAreaWidth()
{
    int digits = 2;
    int lines = document()->blockCount();
    int max = qMax(1, lines);
    while (max >= 100)
    {
        max /= 10;
        ++digits;
    }
    const QFontMetrics fm(document()->defaultFont());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    int space = fm.horizontalAdvance(QLatin1Char('9')) * digits;
#else  // QT_VERSION_CHECK
    int space = fm.width(QLatin1Char('9')) * digits;
#endif // QT_VERSION_CHECK
    space += 8;
    return space;
}

void PlainTextEdit::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    QPainter painter(mpLineNumberArea);
    painter.fillRect(event->rect(), QColor(240, 240, 240));
    QTextBlock block = firstVisibleBlock();
    int        blockNumber = block.blockNumber();
    //计算第一个文本块
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " blockBoundingGeometry(block) = " << blockBoundingGeometry(block)
                       << " -- top = " << top << "contentOffset = " << contentOffset();
    qreal              bottom = top;
    const int          lineNumbersWidth = mpLineNumberArea->width();
    const QFontMetrics fm(document()->defaultFont());
    QTextDocument*     pTextDocument = document();
    while (block.isValid() && top <= event->rect().bottom())
    {
        top = bottom;
        const qreal height = blockBoundingRect(block).height();
        bottom = top + height;
        QTextBlock nextBlock = block.next();
        QTextBlock nextVisibleBlock = nextBlock;
        int        nextVisibleBlockNumber = blockNumber + 1;
        if (!nextVisibleBlock.isVisible())
        {
            nextVisibleBlock = pTextDocument->findBlockByLineNumber(nextVisibleBlock.firstLineNumber());
            nextVisibleBlockNumber = nextVisibleBlock.blockNumber();
        }
        if (bottom < event->rect().top())
        {
            block = nextVisibleBlock;
            blockNumber = nextVisibleBlockNumber;
            continue;
        }

        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number;
            number = QString::number(blockNumber + 1);
            if (blockNumber == textCursor().blockNumber())
            {
                painter.setPen(QColor(64, 64, 64));
            }
            else
            {
                painter.setPen(Qt::gray);
            }
            painter.setFont(document()->defaultFont());
            painter.drawText(0, top, lineNumbersWidth, fm.height(), Qt::AlignRight, number);
        }
        block = nextVisibleBlock;
        blockNumber = nextVisibleBlockNumber;
    }
}

void PlainTextEdit::lineNumberAreaMouseEvent(QMouseEvent* event) {}

void PlainTextEdit::updateLineNumberAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount);
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void PlainTextEdit::updateLineNumberArea(const QRect& rect, int dy)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << mpLineNumberArea->updatesEnabled();
    if (dy)
    {
        // 行号窗体跟随文本窗体滚动
        mpLineNumberArea->scroll(0, dy);
    }
    else
    {
        //文本窗体长度变化，重绘更新
        mpLineNumberArea->update(0, rect.y(), mpLineNumberArea->width(), rect.height());
    }
    // TODO 暂时不清楚作用
    //    if (rect.contains(viewport()->rect()))
    //    {
    //        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    //        updateLineNumberAreaWidth(0);
    //    }
}

void PlainTextEdit::showCompletionItemToolTip(const QModelIndex& index) {}

void PlainTextEdit::insertCompletionItem(const QModelIndex& index)
{
    QVariant      value = index.data(Qt::UserRole);
    CompleterItem completerItem = qvariant_cast<CompleterItem>(value);
    QString       selectiontext = completerItem.mSelect;
    QStringList   completionlength = completerItem.mValue.split("\n");
    QTextCursor   cursor = textCursor();
    cursor.beginEditBlock();
    cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
    cursor.setPosition(cursor.position() - mpCompleter->completionPrefix().length(), QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(completionlength[0]);
    cursor.endEditBlock();
    setTextCursor(cursor);
}

void PlainTextEdit::resizeEvent(QResizeEvent* pEvent)
{
    QPlainTextEdit::resizeEvent(pEvent);
    //使用文本的矩形，它是去掉了间隔的矩形
    QRect cr = contentsRect();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    mpLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void PlainTextEdit::keyPressEvent(QKeyEvent* pEvent)
{
    bool shiftModifier = pEvent->modifiers().testFlag(Qt::ShiftModifier);
    bool controlModifier = pEvent->modifiers().testFlag(Qt::ControlModifier);
    bool isCompleterShortcut = controlModifier && (pEvent->key() == Qt::Key_Space); // CTRL+space
    if (mpCompleter && mpCompleter->popup()->isVisible()) //补全窗口存在
    {
        switch (pEvent->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            pEvent->ignore(); //分发给 QComplete 处理
            qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- ";
            return;
        default:
            break;
        }
    }
    if (pEvent->key() == Qt::Key_Tab || pEvent->key() == Qt::Key_Backtab)
    {
        //TODO 缩进或者
//        indentOrUnindent(pEvent->key() == Qt::Key_Tab);
        return;
    }
    if (!mpCompleter || !isCompleterShortcut)
    {
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- pEvent = " << pEvent->key() ;
        QPlainTextEdit::keyPressEvent(pEvent);
    }
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); //文本jiesu
    QString        completionPrefix = wordUnderCursor();
    int            lastDotIndex = completionPrefix.lastIndexOf('.');
    if (lastDotIndex != -1) //
    {
        completionPrefix = completionPrefix.right(completionPrefix.length() - lastDotIndex - 1);
    }
    if ((!isCompleterShortcut)
        && (pEvent->text().isEmpty() || completionPrefix.length() < 1 || eow.contains(pEvent->text().right(1))))
    {
        mpCompleter->popup()->hide();
        return;
    }
    if (completionPrefix != mpCompleter->completionPrefix())
    {
        mpCompleter->setCompletionPrefix(completionPrefix);
    }
    QRect       cr = cursorRect();
    cr.setWidth(mpCompleter->popup()->sizeHintForColumn(0) + mpCompleter->popup()->verticalScrollBar()->sizeHint().width());
    mpCompleter->complete(cr);
    if (mpCompleter->popup()->selectionModel()->selection().empty())
    {
        mpCompleter->popup()->setCurrentIndex(mpCompleter->completionModel()->index(0, 0));
    }
}

void PlainTextEdit::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        if (event->angleDelta().x() > 0 || event->angleDelta().y() > 0)
        {
#else  // QT_VERSION_CHECK
        if (event->delta() > 0)
        {
#endif // QT_VERSION_CHECK
            zoomIn();
        }
        else
        {
            zoomOut();
        }
    }
    QPlainTextEdit::wheelEvent(event);
}

int PlainTextEdit::firstNonSpace(const QString &text)
{
    int i = 0;
    while (i < text.size())
    {
        if (!text.at(i).isSpace())
        {
            return i;
        }
        ++i;
    }
    return i;
}

LineNumberArea* PlainTextEdit::getMpLineNumberArea() const
{
    return mpLineNumberArea;
}

void PlainTextEdit::insertCompleterTypes(QStringList types)
{
    for (int k = 0; k < types.size(); ++k)
    {
        QStandardItem* pStandardItem = new QStandardItem(types[k]);
        //        pStandardItem->setIcon(ResourceCache::getIcon(":/Resources/icons/completerType.svg"));
        pStandardItem->setData(QVariant::fromValue(CompleterItem(types[k], types[k], "")), Qt::UserRole);
        mpStandardItemModel->appendRow(pStandardItem);
    }
}

QStringList PlainTextEdit::getKeywords()
{
    QStringList keywordsList;
    keywordsList << "algorithm"
                 << "and"
                 << "annotation"
                 << "assert"
                 << "block"
                 << "break"
                 << "class"
                 << "connect"
                 << "connector"
                 << "constant"
                 << "constrainedby"
                 << "der"
                 << "discrete"
                 << "each"
                 << "else"
                 << "elseif"
                 << "elsewhen"
                 << "encapsulated"
                 << "end"
                 << "enumeration"
                 << "equation"
                 << "expandable"
                 << "extends"
                 << "external"
                 << "false"
                 << "final"
                 << "flow"
                 << "for"
                 << "function"
                 << "if"
                 << "import"
                 << "impure"
                 << "in"
                 << "initial"
                 << "inner"
                 << "input"
                 << "loop"
                 << "model"
                 << "not"
                 << "operator"
                 << "or"
                 << "outer"
                 << "output"
                 << "optimization"
                 << "package"
                 << "parameter"
                 << "partial"
                 << "protected"
                 << "public"
                 << "pure"
                 << "record"
                 << "redeclare"
                 << "replaceable"
                 << "return"
                 << "stream"
                 << "then"
                 << "true"
                 << "type"
                 << "when"
                 << "while"
                 << "within";
    return keywordsList;
}

QStringList PlainTextEdit::getTypes()
{
    QStringList typesList;
    typesList << "String"
              << "Integer"
              << "Boolean"
              << "Real";
    return typesList;
}

void PlainTextEdit::initCompleteModel()
{
    mpStandardItemModel->clear();
    QStringList types = PlainTextEdit::getTypes();
    insertCompleterTypes(types);
    QStringList keywords = PlainTextEdit::getKeywords();
    insertCompleterTypes(keywords);
}


void PlainTextEdit::zoomIn()
{
    QFont font = document()->defaultFont();
    qreal fontSize = font.pointSizeF();
    fontSize = fontSize + 1;
    font.setPointSizeF(fontSize);
    document()->setDefaultFont(font);
}

void PlainTextEdit::zoomOut()
{
    QFont font = document()->defaultFont();
    qreal fontSize = font.pointSizeF();
    fontSize = fontSize <= 6 ? fontSize : fontSize - 1;
    font.setPointSizeF(fontSize);
    document()->setDefaultFont(font);
}
QString PlainTextEdit::wordUnderCursor()
{
    int end = textCursor().position();
    int begin = end - 1;
    while (begin >= 0)
    {
        QChar ch = document()->characterAt(begin);
        if (!(ch.isLetterOrNumber() || ch == '.' || ch == '_'))
            break;
        begin--;
    }
    begin++;
    return document()->toPlainText().mid(begin, end - begin);
}




LineNumberArea::LineNumberArea(QWidget* editor)
    : QWidget(editor)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    this->mpEidtor = dynamic_cast<PlainTextEdit*>(editor);
    setStyleSheet("background-color: rgb(255, 170, 255);");
}

QSize LineNumberArea::sizeHint() const
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << mpEidtor->lineNumberAreaWidth();
    return QSize(mpEidtor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    mpEidtor->lineNumberAreaPaintEvent(event);
    //    QWidget::paintEvent(event);
}


CompleterItem::CompleterItem(const QString& key, const QString& value, const QString& select)
    : mKey(key)
    , mValue(value)
    , mSelect(select)
{
    int ind = value.indexOf(select, 0);
    if (ind < 0)
    {
        mDescription = value;
    }
    else
    {
        mDescription = QString("<b>%1</b><i>%2</i>%3").arg(value.left(ind), select, value.right(value.size() - select.size() - ind)).replace("\n", "<br/>");
    }
}

CompleterItem::CompleterItem(const QString& key, const QString& value, const QString& select, const QString& description)
    : mKey(key)
    , mValue(value)
    , mSelect(select)
    , mDescription(description)
{
}

CompleterItem::CompleterItem(const QString& value, const QString& description)
    : mKey(value)
    , mValue(value)
    , mSelect(value)
    , mDescription(description)
{
}
