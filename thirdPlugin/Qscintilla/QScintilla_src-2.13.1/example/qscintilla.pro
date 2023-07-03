# The project file for the QScintilla library.
#
# Copyright (c) 2021 Riverbank Computing Limited <info@riverbankcomputing.com>
# 
# This file is part of QScintilla.
# 
# This file may be used under the terms of the GNU General Public License
# version 3.0 as published by the Free Software Foundation and appearing in
# the file LICENSE included in the packaging of this file.  Please review the
# following information to ensure the GNU General Public License version 3.0
# requirements will be met: http://www.gnu.org/copyleft/gpl.html.
# 
# If you do not wish to use this file under the terms of the GPL version 3.0
# then you may purchase a commercial license.  For more information contact
# info@riverbankcomputing.com.
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


!win32:VERSION = 15.1.1

TEMPLATE = lib
CONFIG += qt warn_off thread exceptions hide_symbols

CONFIG(debug, debug|release) {
    mac: {
        TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}_debug
    } else {
        win32: {
            TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}d
        } else {
            TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}
        }
    }
} else {
    TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}
}

macx:!CONFIG(staticlib) {
    QMAKE_POST_LINK += install_name_tool -id @rpath/$(TARGET1) $(TARGET)
}

INCLUDEPATH += . ../scintilla/include ../scintilla/lexlib ../scintilla/src

!CONFIG(staticlib) {
    DEFINES += QSCINTILLA_MAKE_DLL

    # Comment this in to build a dynamic library supporting multiple
    # architectures on macOS.
    #QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
}
DEFINES += SCINTILLA_QT SCI_LEXER INCLUDE_DEPRECATED_FEATURES
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += widgets
!ios:QT += printsupport
macx:lessThan(QT_MAJOR_VERSION, 6) {
    QT += macextras
}

# Work around QTBUG-39300.
CONFIG -= android_install

# For old versions of GCC.
unix:!macx {
    CONFIG += c++11
}

# Comment this in if you want the internal Scintilla classes to be placed in a
# Scintilla namespace rather than pollute the global namespace.
#DEFINES += SCI_NAMESPACE

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

header.path = $$[QT_INSTALL_HEADERS]
header.files = Qsci
INSTALLS += header

trans.path = $$[QT_INSTALL_TRANSLATIONS]
trans.files = qscintilla_*.qm
INSTALLS += trans

qsci.path = $$[QT_INSTALL_DATA]
qsci.files = ../qsci
INSTALLS += qsci

features.path = $$[QT_HOST_DATA]/mkspecs/features
CONFIG(staticlib) {
    features.files = $$PWD/features_staticlib/qscintilla2.prf
} else {
    features.files = $$PWD/features/qscintilla2.prf
}
INSTALLS += features

HEADERS = \
    ./Qsci/qsciglobal.h \
    ./Qsci/qsciscintilla.h \
    ./Qsci/qsciscintillabase.h \
    ./Qsci/qsciabstractapis.h \
    ./Qsci/qsciapis.h \
    ./Qsci/qscicommand.h \
    ./Qsci/qscicommandset.h \
    ./Qsci/qscidocument.h \
    ./Qsci/qscilexer.h \
    ./Qsci/qscilexeravs.h \
    ./Qsci/qscilexerbatch.h \
    ./Qsci/qscilexercmake.h \
    ./Qsci/qscilexercoffeescript.h \
    ./Qsci/qscilexercpp.h \
    ./Qsci/qscilexercsharp.h \
    ./Qsci/qscilexercss.h \
    ./Qsci/qscilexercustom.h \
    ./Qsci/qscilexerd.h \
    ./Qsci/qscilexerdiff.h \
    ./Qsci/qscilexeredifact.h \
    ./Qsci/qscilexerfortran.h \
    ./Qsci/qscilexerfortran77.h \
    ./Qsci/qscilexerhtml.h \
    ./Qsci/qscilexeridl.h \
    ./Qsci/qscilexerjava.h \
    ./Qsci/qscilexerjavascript.h \
    ./Qsci/qscilexerjson.h \
    ./Qsci/qscilexerlua.h \
    ./Qsci/qscilexermakefile.h \
    ./Qsci/qscilexermarkdown.h \
    ./Qsci/qscilexermatlab.h \
    ./Qsci/qscilexeroctave.h \
    ./Qsci/qscilexerpascal.h \
    ./Qsci/qscilexerperl.h \
    ./Qsci/qscilexerpostscript.h \
    ./Qsci/qscilexerpo.h \
    ./Qsci/qscilexerpov.h \
    ./Qsci/qscilexerproperties.h \
    ./Qsci/qscilexerpython.h \
    ./Qsci/qscilexerruby.h \
    ./Qsci/qscilexerspice.h \
    ./Qsci/qscilexersql.h \
    ./Qsci/qscilexertcl.h \
    ./Qsci/qscilexertex.h \
    ./Qsci/qscilexerverilog.h \
    ./Qsci/qscilexervhdl.h \
    ./Qsci/qscilexerxml.h \
    ./Qsci/qscilexeryaml.h \
    ./Qsci/qscimacro.h \
    ./Qsci/qscistyle.h \
    ./Qsci/qscistyledtext.h \
    ListBoxQt.h \
    Qsci/qscilexermodelica.h \
    SciAccessibility.h \
    SciClasses.h \
    ScintillaQt.h \
    ../scintilla/include/ILexer.h \
    ../scintilla/include/ILoader.h \
    ../scintilla/include/Platform.h \
    ../scintilla/include/Sci_Position.h \
    ../scintilla/include/SciLexer.h \
    ../scintilla/include/Scintilla.h \
    ../scintilla/include/ScintillaWidget.h \
    ../scintilla/lexlib/Accessor.h \
    ../scintilla/lexlib/CharacterCategory.h \
    ../scintilla/lexlib/CharacterSet.h \
    ../scintilla/lexlib/DefaultLexer.h \
    ../scintilla/lexlib/LexAccessor.h \
    ../scintilla/lexlib/LexerBase.h \
    ../scintilla/lexlib/LexerModule.h \
    ../scintilla/lexlib/LexerNoExceptions.h \
    ../scintilla/lexlib/LexerSimple.h \
    ../scintilla/lexlib/OptionSet.h \
    ../scintilla/lexlib/PropSetSimple.h \
    ../scintilla/lexlib/SparseState.h \
    ../scintilla/lexlib/StringCopy.h \
    ../scintilla/lexlib/StyleContext.h \
    ../scintilla/lexlib/SubStyles.h \
    ../scintilla/lexlib/WordList.h \
    ../scintilla/src/AutoComplete.h \
    ../scintilla/src/CallTip.h \
    ../scintilla/src/CaseConvert.h \
    ../scintilla/src/CaseFolder.h \
    ../scintilla/src/Catalogue.h \
    ../scintilla/src/CellBuffer.h \
    ../scintilla/src/CharClassify.h \
    ../scintilla/src/ContractionState.h \
    ../scintilla/src/DBCS.h \
    ../scintilla/src/Decoration.h \
    ../scintilla/src/Document.h \
    ../scintilla/src/EditModel.h \
    ../scintilla/src/Editor.h \
    ../scintilla/src/EditView.h \
    ../scintilla/src/ElapsedPeriod.h \
    ../scintilla/src/ExternalLexer.h \
    ../scintilla/src/FontQuality.h \
    ../scintilla/src/Indicator.h \
    ../scintilla/src/IntegerRectangle.h \
    ../scintilla/src/KeyMap.h \
    ../scintilla/src/LineMarker.h \
    ../scintilla/src/MarginView.h \
    ../scintilla/src/Partitioning.h \
    ../scintilla/src/PerLine.h \
    ../scintilla/src/Position.h \
    ../scintilla/src/PositionCache.h \
    ../scintilla/src/RESearch.h \
    ../scintilla/src/RunStyles.h \
    ../scintilla/src/ScintillaBase.h \
    ../scintilla/src/Selection.h \
    ../scintilla/src/SparseVector.h \
    ../scintilla/src/SplitVector.h \
    ../scintilla/src/Style.h \
    ../scintilla/src/UniConversion.h \
    ../scintilla/src/UniqueString.h \
    ../scintilla/src/ViewStyle.h \
    ../scintilla/src/XPM.h

!ios:HEADERS += ./Qsci/qsciprinter.h

SOURCES = \
    ../scintilla/lexers/LexModelica.cpp \
    qscilexermodelica.cpp \
    qsciscintilla.cpp \
    qsciscintillabase.cpp \
    qsciabstractapis.cpp \
    qsciapis.cpp \
    qscicommand.cpp \
    qscicommandset.cpp \
    qscidocument.cpp \
    qscilexer.cpp \
    qscilexeravs.cpp \
    qscilexerbatch.cpp \
    qscilexercmake.cpp \
    qscilexercoffeescript.cpp \
    qscilexercpp.cpp \
    qscilexercsharp.cpp \
    qscilexercss.cpp \
    qscilexercustom.cpp \
    qscilexerd.cpp \
    qscilexerdiff.cpp \
    qscilexeredifact.cpp \
    qscilexerfortran.cpp \
    qscilexerfortran77.cpp \
    qscilexerhtml.cpp \
    qscilexeridl.cpp \
    qscilexerjava.cpp \
    qscilexerjavascript.cpp \
    qscilexerjson.cpp \
    qscilexerlua.cpp \
    qscilexermakefile.cpp \
    qscilexermarkdown.cpp \
    qscilexermatlab.cpp \
    qscilexeroctave.cpp \
    qscilexerpascal.cpp \
    qscilexerperl.cpp \
    qscilexerpostscript.cpp \
    qscilexerpo.cpp \
    qscilexerpov.cpp \
    qscilexerproperties.cpp \
    qscilexerpython.cpp \
    qscilexerruby.cpp \
    qscilexerspice.cpp \
    qscilexersql.cpp \
    qscilexertcl.cpp \
    qscilexertex.cpp \
    qscilexerverilog.cpp \
    qscilexervhdl.cpp \
    qscilexerxml.cpp \
    qscilexeryaml.cpp \
    qscimacro.cpp \
    qscistyle.cpp \
    qscistyledtext.cpp \
    InputMethod.cpp \
    ListBoxQt.cpp \
    MacPasteboardMime.cpp \
    PlatQt.cpp \
    SciAccessibility.cpp \
    SciClasses.cpp \
    ScintillaQt.cpp \
    ../scintilla/lexlib/Accessor.cpp \
    ../scintilla/lexlib/CharacterCategory.cpp \
    ../scintilla/lexlib/CharacterSet.cpp \
    ../scintilla/lexlib/DefaultLexer.cpp \
    ../scintilla/lexlib/LexerBase.cpp \
    ../scintilla/lexlib/LexerModule.cpp \
    ../scintilla/lexlib/LexerNoExceptions.cpp \
    ../scintilla/lexlib/LexerSimple.cpp \
    ../scintilla/lexlib/PropSetSimple.cpp \
    ../scintilla/lexlib/StyleContext.cpp \
    ../scintilla/lexlib/WordList.cpp \
    ../scintilla/src/AutoComplete.cpp \
    ../scintilla/src/CallTip.cpp \
    ../scintilla/src/CaseConvert.cpp \
    ../scintilla/src/CaseFolder.cpp \
    ../scintilla/src/Catalogue.cpp \
    ../scintilla/src/CellBuffer.cpp \
    ../scintilla/src/CharClassify.cpp \
    ../scintilla/src/ContractionState.cpp \
    ../scintilla/src/DBCS.cpp \
    ../scintilla/src/Decoration.cpp \
    ../scintilla/src/Document.cpp \
    ../scintilla/src/EditModel.cpp \
    ../scintilla/src/Editor.cpp \
    ../scintilla/src/EditView.cpp \
    ../scintilla/src/ExternalLexer.cpp \
    ../scintilla/src/Indicator.cpp \
    ../scintilla/src/KeyMap.cpp \
    ../scintilla/src/LineMarker.cpp \
    ../scintilla/src/MarginView.cpp \
    ../scintilla/src/PerLine.cpp \
    ../scintilla/src/PositionCache.cpp \
    ../scintilla/src/RESearch.cpp \
    ../scintilla/src/RunStyles.cpp \
    ../scintilla/src/ScintillaBase.cpp \
    ../scintilla/src/Selection.cpp \
    ../scintilla/src/Style.cpp \
    ../scintilla/src/UniConversion.cpp \
    ../scintilla/src/ViewStyle.cpp \
    ../scintilla/src/XPM.cpp

!ios:SOURCES += qsciprinter.cpp

TRANSLATIONS = \
    qscintilla_cs.ts \
    qscintilla_de.ts \
    qscintilla_es.ts \
    qscintilla_fr.ts \
    qscintilla_pt_br.ts
