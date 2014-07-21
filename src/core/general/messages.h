#ifndef MESSAGES_H
#define MESSAGES_H

#include <QObject>
#include <QWidget>
#include <QString>

void information(QWidget* apParent, QString aTitle, QString aText);
void warning(QWidget* apParent, QString aTitle, QString aText);
void error(QWidget* apParent, QString aTitle, QString aText);
bool question(QWidget* apParent, QString aTitle, QString aText);

#endif // MESSAGES_H
