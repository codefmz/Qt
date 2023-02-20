// This defines the interface to the QsciLexerModelica class.
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
#ifndef QsciLexerModelica_H
#define QsciLexerModelica_H

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>

#include <QObject>

//! \brief The QsciLexerModelica class encapsulates the Scintilla Matlab file
//! lexer.
class QSCINTILLA_EXPORT QsciLexerModelica : public QsciLexer {
  Q_OBJECT

 public:
  //! This enum defines the meanings of the different styles
  enum {
    //! The default.
    Default = 0,

    //! A comment.
    Comment = 1,

    //! A Primitive type
    Primitive = 2,

    //! A number.
    Number = 3,

    //! An identifier.
    Identifier = 4,

    //! A keyword.
    Keyword = 5,

    //! A keyword.
    Function = 6,

    LineComment = 7,

    MultiLineComment = 8,

    HtmlComment = 9
  };

  //! Construct a QsciLexerModelica with parent \a parent.  \a parent is
  //! typically the QsciScintilla instance.
  QsciLexerModelica(QObject *parent = 0);

  //! Destroys the QsciLexerModelica instance.
  virtual ~QsciLexerModelica();

  //! Returns the name of the language.
  const char *language() const;

  //! Returns the name of the lexer.  Some lexers support a number of
  //! languages.
  const char *lexer() const;

  //! Returns the foreground colour of the text for style number \a style.
  //!
  //! \sa defaultPaper()
  QColor defaultColor(int style) const;

  //! Returns the font for style number \a style.
  QFont defaultFont(int style) const;

  //! Returns the set of keywords for the keyword set \a set recognised
  //! by the lexer as a space separated string.
  const char *keywords(int set) const;

  //! Returns the descriptive name for style number \a style.  If the
  //! style is invalid for this language then an empty QString is returned.
  //! This is intended to be used in user preference dialogs.
  QString description(int style) const;

 private:
  QsciLexerModelica(const QsciLexerModelica &);
  QsciLexerModelica &operator=(const QsciLexerModelica &);
};

#endif
