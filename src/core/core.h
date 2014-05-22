#ifndef CORE_H
#define CORE_H
#include <QtCore/QtGlobal>
#include <QMainWindow>

#if defined(CORE_LIBRARY)
#  define CORE_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define CORE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

extern "C" CORE_LIBRARY_EXPORT QMainWindow* getWindow();

#endif // CORE_H
