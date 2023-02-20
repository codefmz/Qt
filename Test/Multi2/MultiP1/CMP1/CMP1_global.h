#ifndef CMP1_GLOBAL_H
#define CMP1_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CMP1_LIBRARY)
#  define CMP1_EXPORT Q_DECL_EXPORT
#else
#  define CMP1_EXPORT Q_DECL_IMPORT
#endif

#endif // CMP1_GLOBAL_H
