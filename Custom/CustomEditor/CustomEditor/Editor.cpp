#include "Editor.h"
#include <MainWindow.h>
#include <QAbstractItemView>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QTextBlock>
int PlainTextEdit::mTabSize = 4;
int PlainTextEdit::mIndentSize = 2;
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
    BaseEditorDocumentLayout* pModelicaTextDocumentLayout = new BaseEditorDocumentLayout(pTextDocument);
    pTextDocument->setDocumentLayout(pModelicaTextDocumentLayout);
    setDocument(pTextDocument);

    QFont font;
    font.setFamily("Courier New");
    font.setPointSizeF(13);
    setFont(font);

    //功能1：数字行号
    mpLineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    updateLineNumberAreaWidth(0);                 //设置TextEdit的左间距，给行号窗体提供空间
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

    //功能3：

    connect(document(), &QTextDocument::undoAvailable, this, &PlainTextEdit::slotUndoAvailable);
    setUndoRedoEnabled(true);
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateHighlights()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursorPosition()));
    updateHighlights();

    setPlainText("");
    setCompletionCharacters(".");

    mpDocumentMarker = new DocumentMarker(document(), 1);
    setCanHaveBreakpoints(true);
}

void PlainTextEdit::setCanHaveBreakpoints(bool canHaveBreakpoints)
{
    mCanHaveBreakpoints = canHaveBreakpoints;
    mpLineNumberArea->setMouseTracking(canHaveBreakpoints);
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
    if (canHaveBreakpoints())
    {
        space += fm.lineSpacing(); //字体的行间距
    }
    else
    {
        space += 4;
    }
    space += foldBoxWidth(fm);
    return space;
}

void PlainTextEdit::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(mpLineNumberArea);
    painter.fillRect(event->rect(), QColor(240, 240, 240));

    QTextBlock block = firstVisibleBlock();
    int        blockNumber = block.blockNumber();
    //计算第一个文本块
    qreal              top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal              bottom = top;
    const QFontMetrics fm(document()->defaultFont());
    int                fmLineSpacing = fm.lineSpacing();

    int collapseColumnWidth = 4;
    collapseColumnWidth = foldBoxWidth(fm);
    const int lineNumbersWidth = mpLineNumberArea->width() - collapseColumnWidth;

    while (block.isValid() && top <= event->rect().bottom())
    {
        QTextDocument* pTextDocument = document();
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
        /* paint breakpoints */
        TextBlockUserData* pTextBlockUserData = static_cast<TextBlockUserData*>(block.userData());
        if (pTextBlockUserData && canHaveBreakpoints())
        {
            int xoffset = 0;
            foreach (ITextMark* mk, pTextBlockUserData->marks())
            {
                int   radius = fmLineSpacing;
                QRect r(xoffset, top, radius, radius);
                mk->icon().paint(&painter, r, Qt::AlignCenter);
                xoffset += 2;
            }
        }
        /* paint line numbers */
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

        // paint folding markers
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.setPen(Qt::gray);

        // paint folding markers
        TextBlockUserData* nextBlockUserData = BaseEditorDocumentLayout::testUserData(nextBlock);
        bool               drawFoldingControl = nextBlockUserData && BaseEditorDocumentLayout::foldingIndent(block) < nextBlockUserData->foldingIndent();
        bool               drawLine = BaseEditorDocumentLayout::foldingIndent(block) > 0;
        bool               drawEnd
            = drawLine && (!nextBlockUserData || (nextBlockUserData && BaseEditorDocumentLayout::foldingIndent(block) > nextBlockUserData->foldingIndent()));
        int   boxWidth = foldBoxWidth(fm);
        int   size = boxWidth / 4;
        QRect foldingMarkerBox(lineNumbersWidth + size, top + size, 2 * (size) + 1, 2 * (size) + 1);
        QRect foldingLineBox(lineNumbersWidth + size, top, 2 * (size) + 1, height);

        if (drawEnd)
        {
            painter.drawLine(QPointF(foldingLineBox.center().x(), foldingLineBox.top()), foldingLineBox.center());
            painter.drawLine(foldingLineBox.center(), QPointF(foldingLineBox.right(), foldingLineBox.center().y()));
        }

        if (drawLine && !drawEnd)
        {
            painter.drawLine(QPointF(foldingLineBox.center().x(), foldingLineBox.top()), QPointF(foldingLineBox.center().x(), foldingLineBox.bottom()));
        }

        if (drawFoldingControl)
        {
            bool                 expanded = nextBlock.isVisible();
            QStyle*              pStyle = style();
            QStyleOptionViewItem styleOptionViewItem;
            styleOptionViewItem.rect = foldingMarkerBox;
            styleOptionViewItem.state = QStyle::State_Active | QStyle::State_Item | QStyle::State_Children;
            /* For some reason QStyle::PE_IndicatorBranch is not showing up in MAC.
             * So I use QStyle::PE_IndicatorArrowDown and QStyle::PE_IndicatorArrowRight
             * Perhaps this is fixed in newer Qt versions. We will see when we use Qt 5 for MAC.
             */
#ifndef Q_OS_MAC
            if (expanded)
            {
                styleOptionViewItem.state |= QStyle::State_Open;
            }
            pStyle->drawPrimitive(QStyle::PE_IndicatorBranch, &styleOptionViewItem, &painter, mpLineNumberArea);
#else
            styleOptionViewItem.rect.translate(-1, 0);
            if (expanded)
            {
                pStyle->drawPrimitive(QStyle::PE_IndicatorArrowDown, &styleOptionViewItem, &painter, mpLineNumberArea);
            }
            else
            {
                pStyle->drawPrimitive(QStyle::PE_IndicatorArrowRight, &styleOptionViewItem, &painter, mpLineNumberArea);
            }
#endif
        }
        painter.restore();

        block = nextVisibleBlock;
        blockNumber = nextVisibleBlockNumber;
    }
}

void PlainTextEdit::lineNumberAreaMouseEvent(QMouseEvent* event)
{
    QTextCursor        cursor = cursorForPosition(QPoint(0, event->pos().y()));
    const QFontMetrics fm(document()->defaultFont());
    // check mouse click for breakpoints
    if (canHaveBreakpoints())
    {
        int breakPointWidth = fm.lineSpacing();
        // Set whether the mouse cursor is a hand or a normal arrow
        if (event->type() == QEvent::MouseMove)
        {
            bool handCursor = (event->pos().x() <= breakPointWidth);
            if (handCursor != (mpLineNumberArea->cursor().shape() == Qt::PointingHandCursor))
            {
                mpLineNumberArea->setCursor(handCursor ? Qt::PointingHandCursor : Qt::ArrowCursor);
            }
        }
        else if ((event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) && (event->pos().x() <= breakPointWidth))
        {
            /* Do not allow breakpoints if file is not saved. */
            int lineNumber = cursor.blockNumber() + 1;
            if (event->button() == Qt::LeftButton)
            { //! left clicked: add/remove breakpoint
                toggleBreakpoint("", lineNumber);
            }
        }
    }
    // check mouse click for folding markers
    if (true) // foldable
    {
        int boxWidth = foldBoxWidth(fm);
        if (event->button() == Qt::LeftButton && event->pos().x() > mpLineNumberArea->width() - boxWidth)
        {
            if (!cursor.block().next().isVisible())
            {
                toggleBlockVisible(cursor.block());
                moveCursorVisible(false);
            }
            else if (BaseEditorDocumentLayout::canFold(cursor.block()))
            {
                toggleBlockVisible(cursor.block());
                moveCursorVisible(false);
            }
        }
    }
}

void PlainTextEdit::moveCursorVisible(bool ensureVisible)
{
    QTextCursor cursor = textCursor();
    if (!cursor.block().isVisible())
    {
        cursor.setVisualNavigation(true);
        cursor.movePosition(QTextCursor::Up);
        setTextCursor(cursor);
    }
    if (ensureVisible)
    {
        ensureCursorVisible();
    }
}
void PlainTextEdit::updateLineNumberAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount);
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void PlainTextEdit::updateLineNumberArea(const QRect& rect, int dy)
{
    // LineNumberArea 窗口更新
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
    //文本窗口间距更新
    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
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
    //    cursor.insertHtml(QString(R"(<a href="https://www.runoob.com/">%1</a>)").arg(completionlength[0]));
    cursor.endEditBlock();
    setTextCursor(cursor);
}

void PlainTextEdit::updateHighlights()
{
    QList<QTextEdit::ExtraSelection> emptySelect;
    setExtraSelections(emptySelect);
    QList<QTextEdit::ExtraSelection> selections = extraSelections();
    QTextEdit::ExtraSelection        selection;
    QColor                           lineColor = QColor(232, 242, 254);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    selections.append(selection);
    setExtraSelections(selections);
}

void PlainTextEdit::updateCursorPosition()
{
    ensureCursorVisible();
}

void PlainTextEdit::slotUndoAvailable()
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- slot undo available";
}

void PlainTextEdit::resizeEvent(QResizeEvent* pEvent)
{
    QPlainTextEdit::resizeEvent(pEvent);
    //使用文本的矩形，它是去掉了间隔的矩形
    QRect cr = contentsRect();
    mpLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void PlainTextEdit::keyPressEvent(QKeyEvent* pEvent)
{
    bool shiftModifier = pEvent->modifiers().testFlag(Qt::ShiftModifier);
    bool controlModifier = pEvent->modifiers().testFlag(Qt::ControlModifier);
    bool isCompleterShortcut = controlModifier && (pEvent->key() == Qt::Key_Space); // CTRL+space
    bool isCompleterChar = mCompletionCharacters.indexOf(pEvent->key()) != -1;
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
            return;
        default:
            break;
        }
    }

    if (pEvent->key() == Qt::Key_Escape)
    {
        //        if (mpBaseEditor->getFindReplaceWidget()->isVisible()) {
        //            mpBaseEditor->getFindReplaceWidget()->close();
        //        }
        // esc 键
        return;
    }

    if (pEvent->key() == Qt::Key_Tab || pEvent->key() == Qt::Key_Backtab)
    {
        // 缩进或者
        indentOrUnindent(pEvent->key() == Qt::Key_Tab);
        return;
    }
    else if (controlModifier && pEvent->key() == Qt::Key_F)
    {
        // ctrl+f is 查找
        return;
    }
    else if (controlModifier && pEvent->key() == Qt::Key_L)
    {
        // ctrl+l is 跳转行
        return;
    }
    else if (controlModifier && pEvent->key() == Qt::Key_K)
    {
        // ctrl+k is 注释选中行
        return;
    }
    else if (pEvent->matches(QKeySequence::Cut) || pEvent->matches(QKeySequence::Copy))
    {
        // ctrl+x/ctrl+c is pressed.
        //        if (mpBaseEditor->getModelWidget() && mpBaseEditor->getModelWidget()->getLibraryTreeItem()
        //            && ((mpBaseEditor->getModelWidget()->getLibraryTreeItem()->getAccess() <= LibraryTreeItem::nonPackageText)
        //                || (mpBaseEditor->getModelWidget()->getLibraryTreeItem()->getAccess() == LibraryTreeItem::packageText)))
        //        {
        //            return;
        //        }
    }
    else if (pEvent->matches(QKeySequence::Undo))
    {
        // ctrl+z is pressed.
        undo();
        return;
    }
    else if (pEvent->matches(QKeySequence::Redo))
    {
        // ctrl+y is pressed.
        redo();
        return;
    }
    else if (shiftModifier && pEvent->key() == Qt::Key_Home)
    {
        //        handleHomeKey(true);
        return;
    }
    else if (!controlModifier && pEvent->key() == Qt::Key_Home)
    {
        //        handleHomeKey(false);
        return;
    }
    else if (shiftModifier && (pEvent->key() == Qt::Key_Enter || pEvent->key() == Qt::Key_Return))
    {
        /* Ticket #2273. Change shift+enter to enter. */
        pEvent->setModifiers(Qt::NoModifier);
    }
    //正常响应字符串
    if (!mpCompleter || !isCompleterShortcut)
    {
        QPlainTextEdit::keyPressEvent(pEvent);
    }
    /*! @todo We should add formatter classes to handle this based on editor language i.e Modelica or C/C++. */
    if (pEvent->key() == Qt::Key_Enter || pEvent->key() == Qt::Key_Return)
    {
        QTextCursor      cursor = textCursor();
        const QTextBlock previousBlock = cursor.block().previous();
        QString          indentText = previousBlock.text();
        cursor.beginEditBlock();
        cursor.insertText(indentText.left(PlainTextEdit::firstNonSpace(indentText)));
        cursor.endEditBlock();
        setTextCursor(cursor);
    }

    const bool ctrlOrShift = pEvent->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!mpCompleter || (ctrlOrShift && pEvent->text().isEmpty()))
    {
        return;
    }
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); //文本结束
    QString        completionPrefix = wordUnderCursor();
    int            lastDotIndex = completionPrefix.lastIndexOf('.');
    if (lastDotIndex != -1) //
    {
        completionPrefix = completionPrefix.right(completionPrefix.length() - lastDotIndex - 1);
    }
    if ((!isCompleterShortcut) && (pEvent->text().isEmpty() || completionPrefix.length() < 1 || eow.contains(pEvent->text().right(1))))
    {
        mpCompleter->popup()->hide();
        return;
    }
    if (completionPrefix != mpCompleter->completionPrefix())
    {
        mpCompleter->setCompletionPrefix(completionPrefix);
    }
    QRect cr = cursorRect();
    cr.setWidth(mpCompleter->popup()->sizeHintForColumn(0) + mpCompleter->popup()->verticalScrollBar()->sizeHint().width());
    mpCompleter->complete(cr);
    if (mpCompleter->popup()->selectionModel()->selection().empty())
    {
        mpCompleter->popup()->setCurrentIndex(mpCompleter->completionModel()->index(0, 0));
    }
}

void PlainTextEdit::wheelEvent(QWheelEvent* event)
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

void PlainTextEdit::paintEvent(QPaintEvent* e)
{
    //    if (mpCompleterToolTipWidget->isVisible())
    //    {
    //        mpCompleterToolTipWidget->setVisible(mpCompleter->popup()->isVisible());
    //        QModelIndexList modelIndexes = mpCompleter->popup()->selectionModel()->selectedIndexes();
    //        if (!modelIndexes.isEmpty())
    //        {
    //            QRect rect = mpCompleter->popup()->visualRect(modelIndexes.at(0));
    //            mpCompleterToolTipWidget->move(mpCompleter->popup()->mapToGlobal(QPoint(rect.x() + mpCompleter->popup()->width() + 2, rect.y() + 2)));
    //        }
    //    }
    QPlainTextEdit::paintEvent(e);
    QPointF    offset(contentOffset());
    QPainter   painter(viewport());
    QTextBlock block = firstVisibleBlock();

    qreal top = blockBoundingGeometry(block).translated(offset).top();
    qreal bottom = top + blockBoundingRect(block).height();

    QTextCursor cursor = textCursor();
    bool        hasSelection = cursor.hasSelection();
    int         selectionStart = cursor.selectionStart();
    int         selectionEnd = cursor.selectionEnd();

    QTextDocument* pTextDocument = document();

    while (block.isValid() && top <= e->rect().bottom())
    {
        QTextBlock nextBlock = block.next();
        QTextBlock nextVisibleBlock = nextBlock;

        if (!nextVisibleBlock.isVisible())
        {
            // invisible blocks do have zero line count
            nextVisibleBlock = pTextDocument->findBlockByLineNumber(nextVisibleBlock.firstLineNumber());
            // in case our code somewhere did not set the line count of the invisible block to 0
            while (nextVisibleBlock.isValid() && !nextVisibleBlock.isVisible())
            {
                nextVisibleBlock = nextVisibleBlock.next();
            }
        }
        if (block.isVisible() && bottom >= e->rect().top())
        {
            if (nextBlock.isValid() && !nextBlock.isVisible())
            {
                bool selectThis = (hasSelection && nextBlock.position() >= selectionStart && nextBlock.position() < selectionEnd);
                painter.save();
                painter.setFont(document()->defaultFont());
                painter.setPen(QColor(Qt::darkGray));
                if (selectThis)
                {
                    painter.setBrush(palette().highlight());
                }

                QTextLayout* pTextLayout = block.layout();
                QTextLine    line = pTextLayout->lineAt(pTextLayout->lineCount() - 1);
                QRectF       lineRect = line.naturalTextRect().translated(offset.x(), top);
                lineRect.adjust(0, 0, -1, -1);

                QString replacement = QLatin1String("...");
                QString rectReplacement = QLatin1String(" ") + replacement + QLatin1String("); ");

                const QFontMetrics fm(document()->defaultFont());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
                QRectF collapseRect(lineRect.right() + 12, lineRect.top(), fm.horizontalAdvance(rectReplacement), lineRect.height());
#else  // QT_VERSION_CHECK
                QRectF collapseRect(lineRect.right() + 12, lineRect.top(), fm.width(rectReplacement), lineRect.height());
#endif // QT_VERSION_CHECK
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.translate(.5, .5);
                painter.drawRoundedRect(collapseRect.adjusted(0, 0, 0, -1), 3, 3);
                painter.setRenderHint(QPainter::Antialiasing, false);
                painter.translate(-.5, -.5);

                block = nextVisibleBlock.previous();
                if (!block.isValid())
                    block = pTextDocument->lastBlock();

                if (TextBlockUserData* blockUserData = BaseEditorDocumentLayout::testUserData(block))
                {
                    if (blockUserData->foldingEndIncluded())
                    {
                        QString right = block.text().trimmed();
                        if (right.endsWith(QLatin1Char(';')))
                        {
                            right.chop(1);
                            right = right.trimmed();
                            replacement.append(right.right(right.endsWith(QLatin1Char('/')) ? 2 : 1));
                            replacement.append(QLatin1Char(';'));
                        }
                    }
                }

                if (selectThis)
                {
                    painter.setPen(palette().highlightedText().color());
                }
                painter.drawText(collapseRect, Qt::AlignCenter, replacement);
                painter.restore();
            }
        }

        block = nextVisibleBlock;
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    }
}

int PlainTextEdit::firstNonSpace(const QString& text)
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

int PlainTextEdit::lineIndentPosition(const QString& text)
{
    int i = 0;
    while (i < text.size())
    {
        if (!text.at(i).isSpace())
        {
            break;
        }
        ++i;
    }
    int column = columnAt(text, i);
    return i - (column % mIndentSize);
}

int PlainTextEdit::spacesLeftFromPosition(const QString& text, int position)
{
    int i = position;
    while (i > 0)
    {
        if (!text.at(i - 1).isSpace())
        {
            break;
        }
        --i;
    }
    return position - i;
}

int PlainTextEdit::columnAt(const QString& text, int position)
{
    int column = 0;
    for (int i = 0; i < position; ++i)
    {
        if (text.at(i) == QLatin1Char('\t'))
        {
            column = column - (column % mTabSize) + mTabSize;
        }
        else
        {
            ++column;
        }
    }
    return column;
}

int PlainTextEdit::indentedColumn(int column, bool doIndent)
{
    int aligned = (column / mIndentSize) * mIndentSize;
    if (doIndent)
    {
        return aligned + mIndentSize;
    }
    if (aligned < column)
    {
        return aligned;
    }
    return qMax(0, aligned - mIndentSize);
}

QString PlainTextEdit::indentationString(int startColumn, int targetColumn)
{
    targetColumn = qMax(startColumn, targetColumn);
    QString s;
    int     alignedStart = startColumn - (startColumn % mTabSize) + mTabSize;
    if (alignedStart > startColumn && alignedStart <= targetColumn)
    {
        s += QLatin1Char('\t');
        startColumn = alignedStart;
    }
    if (int columns = targetColumn - startColumn)
    {
        int tabs = columns / mTabSize;
        s += QString(tabs, QLatin1Char('\t'));
        s += QString(columns - tabs * mTabSize, QLatin1Char(' '));
    }
    return s;
}

bool PlainTextEdit::cursorIsAtBeginningOfLine(const QTextCursor& cursor)
{
    QString text = cursor.block().text();
    int     fns = firstNonSpace(text);
    return (cursor.position() - cursor.block().position() <= fns);
}

int PlainTextEdit::foldBoxWidth(const QFontMetrics& fm)
{
    const int lineSpacing = fm.lineSpacing();
    return lineSpacing + lineSpacing % 2 + 1;
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

void PlainTextEdit::resetZoom()
{
    QFont font = document()->defaultFont();
    font.setPointSizeF(13);
    document()->setDefaultFont(font);
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

void PlainTextEdit::toggleBreakpoint(const QString fileName, int lineNumber)
{
    BreakpointMarker* pBreakpointMarker;
    auto              iter = lineDocMap.constFind(lineNumber);
    if (iter == lineDocMap.end())
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- lineNumber " << lineNumber;
        /* create a breakpoint marker */
        pBreakpointMarker = new BreakpointMarker(fileName, lineNumber);
        pBreakpointMarker->setEnabled(true);
        /* Add the marker to document marker */
        mpDocumentMarker->addMark(pBreakpointMarker, lineNumber);
        /* insert the breakpoint in BreakpointsWidget */
        lineDocMap.insert(lineNumber, pBreakpointMarker);
    }
    else
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- lineNumber " << lineNumber;
        BreakpointMarker* pBreakpointMarker = iter.value();
        mpDocumentMarker->removeMark(pBreakpointMarker);
        lineDocMap.remove(lineNumber);
        //        pBreakpointsTreeModel->removeBreakpoint(pBreakpointMarker);
    }
}

void PlainTextEdit::toggleBlockVisible(const QTextBlock& block)
{
    BaseEditorDocumentLayout* pBaseEditorDocumentLayout;
    pBaseEditorDocumentLayout = qobject_cast<BaseEditorDocumentLayout*>(document()->documentLayout());
    BaseEditorDocumentLayout::foldOrUnfold(block, BaseEditorDocumentLayout::isFolded(block));
    pBaseEditorDocumentLayout->requestUpdate();
    pBaseEditorDocumentLayout->emitDocumentSizeChanged();
}

void PlainTextEdit::indentOrUnindent(bool doIndent)
{
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- ";
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    // Indent or unindent the selected lines
    if (cursor.hasSelection())
    {
        int            pos = cursor.position();
        int            anchor = cursor.anchor();
        int            start = qMin(anchor, pos);
        int            end = qMax(anchor, pos);
        QTextDocument* doc = document();
        QTextBlock     startBlock = doc->findBlock(start);
        QTextBlock     endBlock = doc->findBlock(end - 1).next();
        // Only one line partially selected.
        if (startBlock.next() == endBlock && (start > startBlock.position() || end < endBlock.position() - 1))
        {
            cursor.removeSelectedText();
        }
        else
        {
            for (QTextBlock block = startBlock; block != endBlock; block = block.next())
            {
                QString text = block.text();
                int     indentPosition = PlainTextEdit::lineIndentPosition(text);
                if (!doIndent && !indentPosition)
                {
                    indentPosition = PlainTextEdit::firstNonSpace(text);
                }
                int targetColumn = PlainTextEdit::indentedColumn(PlainTextEdit::columnAt(text, indentPosition), doIndent);
                cursor.setPosition(block.position() + indentPosition);
                cursor.insertText(PlainTextEdit::indentationString(0, targetColumn));
                cursor.setPosition(block.position());
                cursor.setPosition(block.position() + indentPosition, QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
            }
            cursor.endEditBlock();
            return;
        }
    }
    // Indent or unindent at cursor position
    QTextBlock block = cursor.block();
    QString    text = block.text();
    int        indentPosition = cursor.positionInBlock();
    int        spaces = PlainTextEdit::spacesLeftFromPosition(text, indentPosition);
    int        startColumn = PlainTextEdit::columnAt(text, indentPosition - spaces);
    int        targetColumn = PlainTextEdit::indentedColumn(PlainTextEdit::columnAt(text, indentPosition), doIndent);
    cursor.setPosition(block.position() + indentPosition);
    cursor.setPosition(block.position() + indentPosition - spaces, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(PlainTextEdit::indentationString(startColumn, targetColumn));
    cursor.endEditBlock();
    setTextCursor(cursor);
}

LineNumberArea::LineNumberArea(QWidget* editor)
    : QWidget(editor)
{
    this->mpEidtor = dynamic_cast<PlainTextEdit*>(editor);
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(mpEidtor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event)
{
    mpEidtor->lineNumberAreaPaintEvent(event);
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

TextHighlighter::TextHighlighter(QPlainTextEdit* pPlainTextEdit)
    : QSyntaxHighlighter(pPlainTextEdit->document())
{
    mpPlainTextEdit = pPlainTextEdit;
    initializeSettings();
}

void TextHighlighter::initializeSettings()
{
    QFont font;
    font.setFamily("Courier New");
    font.setPointSizeF(13);
    mpPlainTextEdit->document()->setDefaultFont(font);
    mpPlainTextEdit->setTabStopDistance((qreal)(4 * QFontMetrics(font).horizontalAdvance(QLatin1Char(' '))));
    mHighlightingRules.clear();

    HighlightingRule rule;

    mTextFormat.setForeground(QColor(0, 0, 0));                //文本
    mKeywordFormat.setForeground(QColor(0, 245, 255));         //关键字
    mTypeFormat.setForeground(QColor(255, 10, 10));            //类型
    mSingleLineCommentFormat.setForeground(QColor(0, 150, 0)); //单行注释
    mMultiLineCommentFormat.setForeground(QColor(0, 150, 0));  //多行注释
    mFunctionFormat.setForeground(QColor(0, 0, 255));
    mQuotationFormat.setForeground(QColor(0, 139, 0));
    mNumberFormat.setForeground(QColor(123, 104, 238)); //数字

    //为每个文本项设置 正则匹配
    rule.mPattern = QRegExp("[0-9][0-9]*([.][0-9]*)?([eE][+-]?[0-9]*)?");
    rule.mFormat = mNumberFormat;
    mHighlightingRules.append(rule);

    rule.mPattern = QRegExp("\\b[A-Za-z_][A-Za-z0-9_]*");
    rule.mFormat = mTextFormat;
    mHighlightingRules.append(rule);

    QStringList keywordPatterns = PlainTextEdit::getKeywords();
    foreach (const QString& pattern, keywordPatterns)
    {
        QString newPattern = QString("\\b%1\\b").arg(pattern);
        rule.mPattern = QRegExp(newPattern);
        rule.mFormat = mKeywordFormat;
        mHighlightingRules.append(rule);
    }

    QStringList typePatterns = PlainTextEdit::getTypes();
    foreach (const QString& pattern, typePatterns)
    {
        QString newPattern = QString("\\b%1\\b").arg(pattern);
        rule.mPattern = QRegExp(newPattern);
        rule.mFormat = mTypeFormat;
        mHighlightingRules.append(rule);
    }
}

void TextHighlighter::highlightBlock(const QString& text)
{
    //基本上每增加一个字母，这个函数执行一次
    setCurrentBlockState(0);
    TextBlockUserData* pTextBlockUserData = BaseEditorDocumentLayout::userData(currentBlock());
    if (pTextBlockUserData)
    {
        pTextBlockUserData->setFoldingState(false);
    }
    setFormat(0, text.length(), QColor(0, 0, 0));

    foreach (const HighlightingRule& rule, mHighlightingRules)
    {
        QRegExp expression(rule.mPattern);
        int     index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.mFormat);
            index = expression.indexIn(text, index + length);
        }
    }
    highlightMultiLine(text);
}

void TextHighlighter::highlightMultiLine(const QString& text)
{

    int                index = 0, startIndex = 0;
    int                blockState = previousBlockState();
    bool               foldingState = false;
    QTextBlock         previousTextBlck = currentBlock().previous();
    TextBlockUserData* pPreviousTextBlockUserData = BaseEditorDocumentLayout::userData(previousTextBlck);
    if (pPreviousTextBlockUserData)
    {
        foldingState = pPreviousTextBlockUserData->foldingState();
    }
    QRegExp annotationRegExp("\\bannotation\\b");
    int     annotationIndex = annotationRegExp.indexIn(text);

    // store parentheses info
    Parentheses        parentheses;
    TextBlockUserData* pTextBlockUserData = BaseEditorDocumentLayout::userData(currentBlock());
    if (pTextBlockUserData)
    {
        pTextBlockUserData->clearParentheses();
        pTextBlockUserData->setFoldingIndent(0);
        pTextBlockUserData->setFoldingEndIncluded(false);
    }
    while (index < text.length())
    {
        switch (blockState)
        {
        /* if the block already has single line comment then don't check for multi line comment and quotes. */
        case 1:
            if (text[index] == '/' && index + 1 < text.length() && text[index + 1] == '/')
            {
                index++;
                blockState = 1; /* don't change the blockstate. */
            }
            break;
        case 2:
            if (text[index] == '*' && index + 1 < text.length() && text[index + 1] == '/')
            {
                index++;
                setFormat(startIndex, index - startIndex + 1, mMultiLineCommentFormat);
                blockState = 0;
            }
            break;
        case 3:
            if (text[index] == '\\')
            {
                index++;
            }
            else if (text[index] == '"')
            {
                setFormat(startIndex, index - startIndex + 1, mQuotationFormat);
                blockState = 0;
            }
            break;
        default:
            /* check if single line comment then set the blockstate to 1. */
            if (text[index] == '/' && index + 1 < text.length() && text[index + 1] == '/')
            {
                startIndex = index++;
                setFormat(startIndex, text.length(), mSingleLineCommentFormat);
                blockState = 1;
            }
            else if (text[index] == '/' && index + 1 < text.length() && text[index + 1] == '*')
            {
                startIndex = index++;
                blockState = 2;
            }
            else if (text[index] == '"')
            {
                startIndex = index;
                blockState = 3;
            }
        }

        // if no single line comment, no multi line comment and no quotes then store the parentheses
        if (pTextBlockUserData && (blockState < 1 || blockState > 3))
        {
            if (text[index] == '(' || text[index] == '{' || text[index] == '[')
            {
                parentheses.append(Parenthesis(Parenthesis::Opened, text[index], index));
            }
            else if (text[index] == ')' || text[index] == '}' || text[index] == ']')
            {
                parentheses.append(Parenthesis(Parenthesis::Closed, text[index], index));
            }
        }
        if (pTextBlockUserData && foldingState)
        {
            // if no single line comment, no multi line comment and no quotes then check for annotation end
            if (blockState < 1 || blockState > 3)
            {
                if (text[index] == ';')
                {
                    if (pTextBlockUserData)
                    {
                        QString endText = text.mid(index + 1);
                        /* if we have some text after closing the annotation then we don't want to fold it.
                         * ticket:4310 But if the ending text is just white space then fold it.
                         */
                        if (index == text.length() - 1 || PlainTextEdit::firstNonSpace(endText) == endText.length())
                        {
                            if (annotationIndex < 0)
                            { // if we have one line annotation, we don't want to fold it.
                                pTextBlockUserData->setFoldingIndent(1);
                            }
                            pTextBlockUserData->setFoldingEndIncluded(true);
                        }
                        else
                        {
                            pTextBlockUserData->setFoldingIndent(0);
                        }
                    }
                    foldingState = false;
                }
                else if (pTextBlockUserData && annotationIndex < 0)
                { // if we have one line annotation, we don't want to fold it.
                    pTextBlockUserData->setFoldingIndent(1);
                }
            }
            else if (pTextBlockUserData && annotationIndex < 0)
            { // if we have one line annotation, we don't want to fold it.
                pTextBlockUserData->setFoldingIndent(1);
            }
            else if (pTextBlockUserData && startIndex < annotationIndex)
            { // if we have annotation word before quote or comment block is starting then fold.
                pTextBlockUserData->setFoldingIndent(1);
            }
        }
        else
        {
            // if no single line comment, no multi line comment and no quotes then check for annotation start
            if (blockState < 1 || blockState > 3)
            {
                if (text[index] == 'a' && index + 9 < text.length() && text[index + 1] == 'n' && text[index + 2] == 'n' && text[index + 3] == 'o'
                    && text[index + 4] == 't' && text[index + 5] == 'a' && text[index + 6] == 't' && text[index + 7] == 'i' && text[index + 8] == 'o'
                    && text[index + 9] == 'n')
                {
                    if (index + 9 == text.length() - 1)
                    { // if we just have annotation keyword in the line
                        index = index + 8;
                        foldingState = true;
                    }
                    else if (index + 10 < text.length() && (text[index + 10] == '(' || text[index + 10] == ' '))
                    { // if annotation keyword is followed by '(' or space.
                        index = index + 9;
                        foldingState = true;
                    }
                }
            }
        }
        index++;
    }
    if (pTextBlockUserData)
    {
        pTextBlockUserData->setParentheses(parentheses);
        if (foldingState)
        {
            pTextBlockUserData->setFoldingState(true);
            // Hanldle empty blocks inside annotaiton section
            if (text.isEmpty() && foldingState)
            {
                pTextBlockUserData->setFoldingIndent(1);
            }
        }
        // set text block user data
        setCurrentBlockUserData(pTextBlockUserData);
    }
    switch (blockState)
    {
    case 2:
        setFormat(startIndex, text.length() - startIndex, mMultiLineCommentFormat);
        setCurrentBlockState(2);
        break;
    case 3:
        setFormat(startIndex, text.length() - startIndex, mQuotationFormat);
        setCurrentBlockState(3);
        break;
    }
}

BaseEditorDocumentLayout::BaseEditorDocumentLayout(QTextDocument* document)
    : QPlainTextDocumentLayout(document)
    , mHasBreakpoint(false)
{
}

Parentheses BaseEditorDocumentLayout::parentheses(const QTextBlock& block)
{
    if (TextBlockUserData* userData = testUserData(block))
    {
        return userData->parentheses();
    }
    return Parentheses();
}

TextBlockUserData* BaseEditorDocumentLayout::testUserData(const QTextBlock& block)
{
    return static_cast<TextBlockUserData*>(block.userData());
}

TextBlockUserData* BaseEditorDocumentLayout::userData(const QTextBlock& block)
{
    TextBlockUserData* data = static_cast<TextBlockUserData*>(block.userData());
    if (!data && block.isValid())
    {
        const_cast<QTextBlock&>(block).setUserData((data = new TextBlockUserData));
    }
    return data;
}

/**
 * @file brief: Returns the folding indent of the block
 */
int BaseEditorDocumentLayout::foldingIndent(const QTextBlock& block)
{
    if (TextBlockUserData* userData = testUserData(block))
    {
        return userData->foldingIndent();
    }
    return 0;
}
/**
 * @file brief: Checks if block is foldable
 */
bool BaseEditorDocumentLayout::canFold(const QTextBlock& block)
{
    return (block.next().isValid() && foldingIndent(block.next()) > foldingIndent(block));
}

/**
 * @file brief: Folds/unfolds the block
 */
void BaseEditorDocumentLayout::foldOrUnfold(const QTextBlock& block, bool unfold)
{
    if (!canFold(block))
    {
        return;
    }
    QTextBlock b = block.next();
    int        indent = foldingIndent(block);
    while (b.isValid() && foldingIndent(b) > indent && (unfold || b.next().isValid()))
    {
        b.setVisible(unfold);
        b.setLineCount(unfold ? qMax(1, b.layout()->lineCount()) : 0);
        if (unfold)
        { // do not unfold folded sub-blocks
            if (isFolded(b) && b.next().isValid())
            {
                int jndent = foldingIndent(b);
                b = b.next();
                while (b.isValid() && foldingIndent(b) > jndent)
                {
                    b = b.next();
                }
                continue;
            }
        }
        b = b.next();
    }
    setFolded(block, !unfold);
}

void BaseEditorDocumentLayout::setFolded(const QTextBlock& block, bool folded)
{
    if (folded)
    {
        userData(block)->setFolded(true);
    }
    else if (TextBlockUserData* userData = testUserData(block))
    {
        return userData->setFolded(false);
    }
}

bool BaseEditorDocumentLayout::isFolded(const QTextBlock& block)
{
    if (TextBlockUserData* userData = testUserData(block))
    {
        return userData->folded();
    }
    return false;
}

bool BaseEditorDocumentLayout::hasParentheses(const QTextBlock& block)
{
    if (TextBlockUserData* userData = testUserData(block))
    {
        return userData->hasParentheses();
    }
    return false;
}

void BaseEditorDocumentLayout::setFoldingIndent(const QTextBlock& block, int indent)
{
    if (indent == 0)
    {
        if (TextBlockUserData* userData = testUserData(block))
        {
            userData->setFoldingIndent(0);
        }
    }
    else
    {
        userData(block)->setFoldingIndent(indent);
    }
}

DocumentMarker::DocumentMarker(QTextDocument* doc, int lineStartNumber)
    : ITextMarkable(doc)
    , mpTextDocument(doc)
    , mLineStartNumber(lineStartNumber)
{
}

bool DocumentMarker::addMark(ITextMark* mark, int line)
{
    if (line >= 1)
    {
        int                       blockNumber = line - 1;
        BaseEditorDocumentLayout* docLayout = qobject_cast<BaseEditorDocumentLayout*>(mpTextDocument->documentLayout());
        if (!docLayout)
        {
            return false;
        }
        QTextBlock block = mpTextDocument->findBlockByNumber(mLineStartNumber > 0 ? line - mLineStartNumber : blockNumber);
        if (block.isValid())
        {
            TextBlockUserData* userData = BaseEditorDocumentLayout::userData(block);
            userData->addMark(mark);
            mark->updateLineNumber(line);
            mark->updateBlock(block);
            docLayout->mHasBreakpoint = true;
            docLayout->requestUpdate();
            return true;
        }
    }
    return false;
}

TextMarks DocumentMarker::marksAt(int line) const
{
    if (line >= 1)
    {
        int        blockNumber = line - 1;
        QTextBlock block = mpTextDocument->findBlockByNumber(blockNumber);
        if (block.isValid())
        {
            if (TextBlockUserData* userData = BaseEditorDocumentLayout::testUserData(block))
            {
                return userData->marks();
            }
        }
    }
    return TextMarks();
}

void DocumentMarker::removeMark(ITextMark* mark)
{
    bool       needUpdate = false;
    QTextBlock block = mpTextDocument->begin();
    while (block.isValid())
    {
        if (TextBlockUserData* data = static_cast<TextBlockUserData*>(block.userData()))
        {
            needUpdate |= data->removeMark(mark);
        }
        block = block.next();
    }
    if (needUpdate)
    {
        updateMark(0);
    }
}

bool DocumentMarker::hasMark(ITextMark* mark) const
{
    QTextBlock block = mpTextDocument->begin();
    while (block.isValid())
    {
        if (TextBlockUserData* data = static_cast<TextBlockUserData*>(block.userData()))
        {
            if (data->hasMark(mark))
            {
                return true;
            }
        }
        block = block.next();
    }
    return false;
}

void DocumentMarker::updateMark(ITextMark* mark)
{
    Q_UNUSED(mark)
    BaseEditorDocumentLayout* docLayout = qobject_cast<BaseEditorDocumentLayout*>(mpTextDocument->documentLayout());
    if (docLayout)
    {
        docLayout->requestUpdate();
    }
}

void DocumentMarker::updateBreakpointsLineNumber()
{
    QTextBlock block = mpTextDocument->begin();
    int        blockNumber = 0;
    while (block.isValid())
    {
        if (const TextBlockUserData* userData = BaseEditorDocumentLayout::testUserData(block))
        {
            foreach (ITextMark* mrk, userData->marks())
            {
                mrk->updateLineNumber(mLineStartNumber > 0 ? blockNumber + mLineStartNumber : blockNumber + 1);
            }
        }
        block = block.next();
        ++blockNumber;
    }
}

void DocumentMarker::updateBreakpointsBlock(const QTextBlock& block)
{
    if (const TextBlockUserData* userData = BaseEditorDocumentLayout::testUserData(block))
        foreach (ITextMark* mrk, userData->marks())
        {
            mrk->updateBlock(block);
        }
}

TextBlockUserData::~TextBlockUserData()
{
    TextMarks marks = _marks;
    _marks.clear();
    foreach (ITextMark* mk, marks)
    {
        mk->removeFromEditor();
    }
}

/**
 * @file brief: Checks the open Parenthesis for any mismatch
 */
TextBlockUserData::MatchType TextBlockUserData::checkOpenParenthesis(QTextCursor* cursor, QChar c)
{
    QTextBlock block = cursor->block();
    if (!BaseEditorDocumentLayout::hasParentheses(block))
    {
        return NoMatch;
    }

    Parentheses parentheses = BaseEditorDocumentLayout::parentheses(block);
    Parenthesis openParenthesis, closedParenthesis;
    QTextBlock  closedParenthesisBlock = block;
    const int   cursorPos = cursor->position() - closedParenthesisBlock.position();
    int         i = 0;
    int         ignore = 0;
    bool        foundOpen = false;
    for (;;)
    {
        if (!foundOpen)
        {
            if (i >= parentheses.count())
                return NoMatch;
            openParenthesis = parentheses.at(i);
            if (openParenthesis.pos != cursorPos)
            {
                ++i;
                continue;
            }
            else
            {
                foundOpen = true;
                ++i;
            }
        }

        if (i >= parentheses.count())
        {
            for (;;)
            {
                closedParenthesisBlock = closedParenthesisBlock.next();
                if (!closedParenthesisBlock.isValid())
                    return NoMatch;
                if (BaseEditorDocumentLayout::hasParentheses(closedParenthesisBlock))
                {
                    parentheses = BaseEditorDocumentLayout::parentheses(closedParenthesisBlock);
                    break;
                }
            }
            i = 0;
        }

        closedParenthesis = parentheses.at(i);
        if (closedParenthesis.type == Parenthesis::Opened)
        {
            ignore++;
            ++i;
            continue;
        }
        else
        {
            if (ignore > 0)
            {
                ignore--;
                ++i;
                continue;
            }

            cursor->clearSelection();
            cursor->setPosition(closedParenthesisBlock.position() + closedParenthesis.pos + 1, QTextCursor::KeepAnchor);

            if ((c == QLatin1Char('{') && closedParenthesis.chr != QLatin1Char('}')) || (c == QLatin1Char('(') && closedParenthesis.chr != QLatin1Char(')'))
                || (c == QLatin1Char('[') && closedParenthesis.chr != QLatin1Char(']')))
            {
                return Mismatch;
            }

            return Match;
        }
    }
}

/*!
 * \brief TextBlockUserData::checkClosedParenthesis
 * Checks the close Parenthesis for any mismatch
 * \param cursor
 * \param c
 * \return
 */
TextBlockUserData::MatchType TextBlockUserData::checkClosedParenthesis(QTextCursor* cursor, QChar c)
{
    QTextBlock block = cursor->block();
    if (!BaseEditorDocumentLayout::hasParentheses(block))
    {
        return NoMatch;
    }

    Parentheses parentheses = BaseEditorDocumentLayout::parentheses(block);
    Parenthesis openParenthesis, closedParenthesis;
    QTextBlock  openParenthesisBlock = block;
    const int   cursorPos = cursor->position() - openParenthesisBlock.position();
    int         i = parentheses.count() - 1;
    int         ignore = 0;
    bool        foundClosed = false;
    for (;;)
    {
        if (!foundClosed)
        {
            if (i < 0)
                return NoMatch;
            closedParenthesis = parentheses.at(i);
            if (closedParenthesis.pos != cursorPos - 1)
            {
                --i;
                continue;
            }
            else
            {
                foundClosed = true;
                --i;
            }
        }

        if (i < 0)
        {
            for (;;)
            {
                openParenthesisBlock = openParenthesisBlock.previous();
                if (!openParenthesisBlock.isValid())
                    return NoMatch;

                if (BaseEditorDocumentLayout::hasParentheses(openParenthesisBlock))
                {
                    parentheses = BaseEditorDocumentLayout::parentheses(openParenthesisBlock);
                    break;
                }
            }
            i = parentheses.count() - 1;
        }

        openParenthesis = parentheses.at(i);
        if (openParenthesis.type == Parenthesis::Closed)
        {
            ignore++;
            --i;
            continue;
        }
        else
        {
            if (ignore > 0)
            {
                ignore--;
                --i;
                continue;
            }

            cursor->clearSelection();
            cursor->setPosition(openParenthesisBlock.position() + openParenthesis.pos, QTextCursor::KeepAnchor);

            if ((c == QLatin1Char('}') && openParenthesis.chr != QLatin1Char('{')) || (c == QLatin1Char(')') && openParenthesis.chr != QLatin1Char('('))
                || (c == QLatin1Char(']') && openParenthesis.chr != QLatin1Char('[')))
            {
                return Mismatch;
            }
            return Match;
        }
    }
}

/*!
 * \brief TextBlockUserData::matchCursorBackward
 * Matches the parentheses in the backward direction.
 * \param cursor
 * \return
 */
TextBlockUserData::MatchType TextBlockUserData::matchCursorBackward(QTextCursor* cursor)
{
    cursor->clearSelection();
    const QTextBlock block = cursor->block();
    if (!BaseEditorDocumentLayout::hasParentheses(block))
    {
        return NoMatch;
    }

    const int                         relPos = cursor->position() - block.position();
    Parentheses                       parentheses = BaseEditorDocumentLayout::parentheses(block);
    const Parentheses::const_iterator cend = parentheses.constEnd();
    for (Parentheses::const_iterator it = parentheses.constBegin(); it != cend; ++it)
    {
        const Parenthesis& parenthesis = *it;
        if (parenthesis.pos == relPos - 1 && parenthesis.type == Parenthesis::Closed)
        {
            return checkClosedParenthesis(cursor, parenthesis.chr);
        }
    }
    return NoMatch;
}

/*!
 * \brief TextBlockUserData::matchCursorForward
 * Matches the parentheses in the forward direction.
 * \param cursor
 * \return
 */
TextBlockUserData::MatchType TextBlockUserData::matchCursorForward(QTextCursor* cursor)
{
    cursor->clearSelection();
    const QTextBlock block = cursor->block();
    if (!BaseEditorDocumentLayout::hasParentheses(block))
    {
        return NoMatch;
    }

    const int                         relPos = cursor->position() - block.position();
    Parentheses                       parentheses = BaseEditorDocumentLayout::parentheses(block);
    const Parentheses::const_iterator cend = parentheses.constEnd();
    for (Parentheses::const_iterator it = parentheses.constBegin(); it != cend; ++it)
    {
        const Parenthesis& parenthesis = *it;
        if (parenthesis.pos == relPos && parenthesis.type == Parenthesis::Opened)
        {
            return checkOpenParenthesis(cursor, parenthesis.chr);
        }
    }
    return NoMatch;
}

BreakpointMarker::BreakpointMarker(const QString& fileName, int lineNumber)
    : ITextMark()
    , mpFileName(fileName)
    , mpLineNumber(lineNumber)
    , mEnabled(true)
    , mIgnoreCount(0)
    , mCondition("")
{
}

QIcon BreakpointMarker::icon() const
{
    return isEnabled() ? QIcon(":/Resources/icons/breakpoint_enabled.svg") : QIcon(":/Resources/icons/breakpoint_disabled.svg");
}

void BreakpointMarker::updateLineNumber(int lineNumber)
{
    if (lineNumber != mpLineNumber)
    {
        //        mpBreakpointsTreeModel->updateBreakpoint(this, lineNumber);
        mpLineNumber = lineNumber;
    }
}

void BreakpointMarker::updateBlock(const QTextBlock& block)
{
    if (mpLineText != block.text())
    {
        mpLineText = block.text();
    }
}

void BreakpointMarker::removeFromEditor()
{
    //    mpBreakpointsTreeModel->removeBreakpoint(this);
}

void BreakpointMarker::documentClosing()
{
    // todo: impl
}
