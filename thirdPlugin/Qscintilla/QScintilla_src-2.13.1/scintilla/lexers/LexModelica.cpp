/**
* @projectName  scintilla
* @brief        support the modelica highlight
* @author       qiucx
* @date         2022-02-18
*/
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "CharacterSet.h"
#include "ILexer.h"
#include "LexAccessor.h"
#include "LexerModule.h"
#include "SciLexer.h"
#include "Scintilla.h"
#include "StyleContext.h"
#include "WordList.h"
#include <QDebug>
#include "DefaultLexer.h"
using namespace Scintilla;

static const char *const WordListDesc[] = {"Modelica keywords", 0};
static const char *const Keywords = {
    "if else elseif elsewhen when while then end for in loop break return and "
    "or not block class connector external function model operator package "
    "record type enumeration discrete parameter constant protected public "
    "operator record operator function expandable connector pure function "
    "impure function partial final flow stream input output inner outer "
    "initial encapsulated false true algorithm equation redeclare replaceable "
    "constrainedby annotation connect der extends import within each"};
static const char *const Primitives = {
    "Real Integer Boolean String"};
static const char *const SingleLineString = "//";
static const char *const MultiLineString = "/*";

QVector<QRegExp> mKeywordsRules;
QVector<QRegExp> mPrimtiveRules;
//QVector<QRegExp> mPrimtiveRules;
class LexModelica : public DefaultLexer {
 public:
  LexModelica() {}
  virtual ~LexModelica() {}
  int SCI_METHOD Version() const { return 1; }
  void SCI_METHOD Release() { delete this; }
  const char *SCI_METHOD PropertyNames() { return NULL; }
  int SCI_METHOD PropertyType(const char *name) { return -1; }
  const char *SCI_METHOD DescribeProperty(const char *name) { return NULL; }
  Sci_Position SCI_METHOD PropertySet(const char *key, const char *val) { return -1; }
  const char *SCI_METHOD DescribeWordListSets() { return WordListDesc[0]; }
  Sci_Position SCI_METHOD WordListSet(int n, const char *wl) { return -1; }
  void SCI_METHOD Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle,
                      IDocument *pAccess);
  void SCI_METHOD Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle,
                       IDocument *pAccess);
  void *SCI_METHOD PrivateCall(int operation, void *pointer) { return NULL; }
  static ILexer *LexerFactoryModelica() { return new LexModelica(); }
  void initRules() {}
};
void SCI_METHOD LexModelica ::Lex(Sci_PositionU startPos, Sci_Position lengthDoc,
                                int initStyle, IDocument *pAccess) {
  const unsigned int MAX_STR_LEN = 100;
  LexAccessor styler(pAccess);
  StyleContext scCTX(startPos, lengthDoc, initStyle, styler);
  bool pointInNumberUsed = false;
  WordList modelicaKeywords;
  WordList modelicaPrimitives;
  WordList modelicaAllSpecialWords;
  char *tmpStr;
  char *allSpecialWords = (char *) malloc(1 + strlen(Keywords)+ strlen(Primitives));
  CharacterSet setAllSpecialChars(CharacterSet::setAlpha, "\'");
  modelicaAllSpecialWords.Set(allSpecialWords);
  CharacterSet setKeywordChars(CharacterSet::setAlpha, "\'");
  modelicaKeywords.Set(Keywords);
  CharacterSet setPrimitiveChars(CharacterSet::setAlpha, "\'");
  modelicaPrimitives.Set(Primitives);
  tmpStr = new char[MAX_STR_LEN];
  //bool htmlStart = false;
  for (; scCTX.More(); scCTX.Forward()) {
    //bool atEOL = ((scCTX.ch == '\r' && scCTX.chNext != '\n') || (scCTX.ch == '\n'));
    //qDebug()<<"atEOL is"<<atEOL;
    switch (scCTX.state) {
      //qDebug()<<"state is "<<scCTX.state;
      case SCE_MODELICA_FUNCTION:
          if (scCTX.Match(')')) { //
              scCTX.Forward();
              scCTX.ForwardSetState(SCE_MODELICA_DEFAULT);
          };
          break;
      case SCE_MODELICA_COMMENT:
          /*if (htmlStart){
              if (scCTX.Match('>') && scCTX.chNext == '\"') {
                  scCTX.Forward();
                  scCTX.ForwardSetState(SCE_MODELICA_DEFAULT);
                  htmlStart = false;
              }
          } else*/
          if (scCTX.Match('\"')) { //
                  //qDebug()<<"comments is Default";
                  scCTX.ForwardSetState(SCE_MODELICA_DEFAULT);
                  scCTX.Forward();
          };
          break;
      case SCE_MODELICA_LINE_COMMENT:
        if (scCTX.ch == '\r' || scCTX.ch == '\n') { //
            //qDebug()<<"SingleLine end";
            scCTX.SetState(SCE_MODELICA_DEFAULT);
            scCTX.Forward();
        };
        break;
      case SCE_MODELICA_MULTILINE_COMMENT:
        if (scCTX.Match('*', '/')) { //
            //qDebug()<<"MultiLine end";
            scCTX.Forward();
            scCTX.ForwardSetState(SCE_MODELICA_DEFAULT);
        };
        break;
      case SCE_MODELICA_HTML_COMMENT:
          if (scCTX.Match('>') && scCTX.chNext == '\"') { //
              //qDebug()<<"MultiLine end";
              scCTX.Forward();
              scCTX.ForwardSetState(SCE_MODELICA_DEFAULT);
          };
          break;
      case SCE_MODELICA_NUMBER:
        if (scCTX.Match('.')) {
          if (pointInNumberUsed) {
            scCTX.SetState(SCE_MODELICA_DEFAULT);
          } else {
            pointInNumberUsed = true;
          };
        } else {
          if (!IsADigit(scCTX.ch)) {
            scCTX.SetState(SCE_MODELICA_DEFAULT);
          };
        };
        break;
      case SCE_MODELICA_IDENTIFIER:
        if (!setKeywordChars.Contains(scCTX.ch)) {
          memset(tmpStr, 0, sizeof(char) * MAX_STR_LEN);
          scCTX.GetCurrent(tmpStr, MAX_STR_LEN);
          if (modelicaKeywords.InList(tmpStr) && (scCTX.ch == ' ' || scCTX.ch =='\r' || scCTX.ch =='\n' || scCTX.ch =='\t' || scCTX.ch == NULL || scCTX.ch == '(')) {
              //qDebug()<<"this is keyword>>"<<tmpStr;
            scCTX.ChangeState(SCE_MODELICA_KEYWORD);
          } else if (modelicaPrimitives.InList(tmpStr)) {
              //qDebug()<<"this is primitive>>"<<tmpStr;
              scCTX.ChangeState(SCE_MODELICA_PRIMITIVE);
          } else if(scCTX.ch == '(') {
              //qDebug()<<"this is function>>"<<tmpStr;
              scCTX.ChangeState(SCE_MODELICA_FUNCTION);
          }
          scCTX.SetState(SCE_MODELICA_DEFAULT);

        };
        break;
    };

    if(scCTX.state == SCE_MODELICA_DEFAULT) {
        if (scCTX.Match('\"')) { // match the " then the state turn to SCE_MODELICA_COMMENT
            //scCTX.Forward();
            scCTX.SetState(SCE_MODELICA_COMMENT);
            if (scCTX.chNext == '<') {
                //htmlStart = true;
                scCTX.SetState(SCE_MODELICA_HTML_COMMENT);
            }
        }
        else if (scCTX.Match(SingleLineString) && scCTX.state != SCE_MODELICA_COMMENT) { // match the " then the state turn to SCE_MODELICA_COMMENT
            //scCTX.Forward();
            //qDebug()<<">>>>>>>>>>>>>>>>>turn to SingleLine";
            scCTX.SetState(SCE_MODELICA_LINE_COMMENT);
        }
        else if (scCTX.Match('/', '*')) { // match the " then the state turn to SCE_MODELICA_COMMENT
            //scCTX.Forward();
            //qDebug()<<">>>>>>>>>>>>>>turn to MultiLine";
            scCTX.SetState(SCE_MODELICA_MULTILINE_COMMENT);
            //scCTX.Forward();
        }
        else if (IsADigit(scCTX.ch) ||
            (scCTX.Match('-') && IsADigit(scCTX.chNext))) {
            scCTX.SetState(SCE_MODELICA_NUMBER);
            pointInNumberUsed = false;
        }
        else if (setKeywordChars.Contains(scCTX.ch) && (scCTX.chPrev == ';' || scCTX.chPrev == ' ' || scCTX.chPrev =='\t' || scCTX.chPrev =='\r' || scCTX.chPrev =='\n' || scCTX.chPrev == NULL || scCTX.chPrev == '(' || scCTX.chPrev == '{')) {
            scCTX.SetState(SCE_MODELICA_IDENTIFIER);
        }
    }

  };
  scCTX.Complete();
  delete[] tmpStr;
  free(allSpecialWords);
}
void SCI_METHOD LexModelica::Fold(Sci_PositionU startPos, Sci_Position lengthDoc,
                                int initStyle, IDocument *pAccess) {
  LexAccessor styler(pAccess);
  unsigned int endPos = startPos + lengthDoc;
  startPos  = 0;  //scan from 0 line every time;
  char chNext = styler[startPos];
  int lineCurrent = styler.GetLine(startPos);
  int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
  int levelCurrent = levelPrev;
  char ch;
  bool atEOL;

  for (unsigned int i = startPos; i < endPos; i++) {
    ch = chNext;
    chNext = styler.SafeGetCharAt(i + 1);
    atEOL = ((ch == '\r' && chNext != '\n') || (ch == '\n'));
    bool isRightBrace = false;
    if (ch == '(') {
      levelCurrent++;
    };
    if (ch == ')') {
      levelCurrent--;
      isRightBrace = true;
    };
    if (atEOL || (i == (endPos - 1))) {
      int lev = levelPrev;
      if (levelCurrent > levelPrev) {
        lev |= SC_FOLDLEVELHEADERFLAG;
      };
      if (lev != styler.LevelAt(lineCurrent)) {
        styler.SetLevel(lineCurrent, lev);
      };

      if (isRightBrace){
        int rightNum = styler.GetLine(i);
//        qDebug()<<"rightNum::"<<rightNum;
//        qDebug()<<"styler.LevelAt(rightNum):"<<styler.LevelAt(rightNum)<<"::styler.LevelAt(rightNum+1):"<<styler.LevelAt(rightNum+1);
        if(styler.LevelAt(rightNum) <= styler.LevelAt(rightNum+1))
            styler.SetLevel(rightNum+1, styler.LevelAt(rightNum)-1);
      }
      lineCurrent++;
      levelPrev = levelCurrent;
    };
  };
}

LexerModule lmModelica(SCLEX_MODELICA, LexModelica::LexerFactoryModelica, "Modelica",
                   WordListDesc);
