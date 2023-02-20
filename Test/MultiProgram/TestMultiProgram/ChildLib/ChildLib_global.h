#ifndef CHILDLIB_GLOBAL_H
#define CHILDLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CHILDLIB_LIBRARY)
#  define CHILDLIB_EXPORT Q_DECL_EXPORT
#else
#  define CHILDLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // CHILDLIB_GLOBAL_H
