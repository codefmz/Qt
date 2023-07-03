// This module implements the QsciLexerModelica class.
//
// Copyright (c) 2021 Riverbank Computing Limited <info@riverbankcomputing.com>
//
// This file is part of QScintilla.
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
//
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

/**
* @projectName  qscintilla
* @brief        support the modelica highlight
* @author       qiucx
* @date         2022-02-18
*/
#include "Qsci/qscilexermodelica.h"

#include <qcolor.h>
#include <qfont.h>

// The ctor.
QsciLexerModelica::QsciLexerModelica(QObject *parent) : QsciLexer(parent) {}

// The dtor.
QsciLexerModelica::~QsciLexerModelica() {}

// Returns the language name.
const char *QsciLexerModelica::language() const { return "Modelica"; }

// Returns the lexer name.
const char *QsciLexerModelica::lexer() const { return "Modelica"; }

// Returns the foreground colour of the text for a style.
QColor QsciLexerModelica::defaultColor(int style) const {
  switch (style) {
    case Default:
      return QColor(0x00, 0x00, 0x00);
    case LineComment:
    case MultiLineComment:
    case HtmlComment:
    case Comment:
      return QColor(0x00, 0x7f, 0x00);

    case Primitive:
      return QColor(138, 43, 226);

    case Number:
      return QColor(255, 0, 255);

    case Keyword:
      return QColor(178, 34, 34);

    case Function:
        return QColor(0, 0, 255);
  }

  return QsciLexer::defaultColor(style);
}

// Returns the font of the text for a style.
QFont QsciLexerModelica::defaultFont(int style) const {
  QFont f;

  switch (style) {
    case Comment:
#if defined(Q_OS_WIN)
      f = QFont("Comic Sans MS", 9);
#elif defined(Q_OS_MAC)
      f = QFont("Comic Sans MS", 12);
#else
      f = QFont("Bitstream Vera Serif", 9);
#endif
      break;

    case Keyword:
      f = QsciLexer::defaultFont(style);
      //f.setBold(true);
      break;

    default:
      f = QsciLexer::defaultFont(style);
  }

  return f;
}

// Returns the set of keywords.
const char *QsciLexerModelica::keywords(int set) const {
  if (set == 1)
    return "break case catch continue else elseif end for function "
           "global if otherwise persistent return switch try while";

  return 0;
}

// Returns the user name of a style.
QString QsciLexerModelica::description(int style) const {
  switch (style) {
    case Default:
      return tr("Default");

    case Comment:
    case LineComment:
    case MultiLineComment:
    case HtmlComment:
      return tr("Comment");

    case Primitive:
      return tr("Primitive");

    case Number:
      return tr("Number");

    case Keyword:
      return tr("Keyword");

    case Identifier:
      return tr("Identifier");

    case Function:
        return tr("Function");
  }

  return QString();
}
