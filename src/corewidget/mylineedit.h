#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit: public QLineEdit
{
    Q_OBJECT
    bool mIsEdit;
    QString mText;
    EchoMode mType;
    QPalette mTextPalette;
    QPalette mWorkTextPalette;
public:
    MyLineEdit(QString aText, EchoMode aType, QWidget* apPerent = 0);
    QString	text() const;
protected:
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);
};

#endif // MYLINEEDIT_H
