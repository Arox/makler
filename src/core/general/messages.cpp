#include "messages.h"
#include "language.h"

#include <QMessageBox>
#include <QPushButton>

void information(QWidget* apParent, QString aTitle, QString aText)
{
    QMessageBox vBox (QMessageBox::Information, aTitle, aText, QMessageBox::NoButton, apParent);
    vBox.addButton(TRANSLATE("Ok"), QMessageBox::AcceptRole)->setObjectName("mpOk");

    vBox.exec();
}

void warning(QWidget* apParent, QString aTitle, QString aText)
{
    QMessageBox vBox (QMessageBox::Warning, aTitle, aText, QMessageBox::NoButton, apParent);
    vBox.addButton(TRANSLATE("Ok"), QMessageBox::AcceptRole)->setObjectName("mpOk");
    vBox.exec();
}

void error(QWidget* apParent, QString aTitle, QString aText)
{
    QMessageBox vBox (QMessageBox::Critical, aTitle, aText, QMessageBox::NoButton, apParent);
    vBox.addButton(TRANSLATE("Ok"), QMessageBox::AcceptRole)->setObjectName("mpOk");
    vBox.exec();
}

bool question(QWidget* apParent, QString aTitle, QString aText)
{
    QMessageBox vBox (QMessageBox::Question, aTitle, aText, QMessageBox::NoButton, apParent);
    vBox.addButton(TRANSLATE("Нет"), QMessageBox::YesRole)->setObjectName("mpNo");
    QPushButton* vpYes = new QPushButton(TRANSLATE("Да"), apParent);
    vpYes->setObjectName("mpYes");
    vpYes->setProperty("color", "true");
    vBox.addButton(vpYes, QMessageBox::NoRole);

    vBox.exec();
    delete vpYes;
    return vBox.clickedButton() == vpYes;
}
