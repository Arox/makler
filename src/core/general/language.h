#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QApplication>

#ifdef WINDOWS_OS
#define TRANSLATE(x) QApplication::translate("TRANSLATE", x, 0)
#else
#define TRANSLATE(x) QApplication::translate("TRANSLATE", x, 0, QApplication::UnicodeUTF8)
#endif

#endif // LANGUAGE_H
