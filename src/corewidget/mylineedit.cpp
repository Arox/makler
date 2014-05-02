#include "mylineedit.h"

MyLineEdit::MyLineEdit(QString aText, EchoMode aType, QWidget* apPerent):
    QLineEdit(apPerent)
  , mIsEdit(false)
  , mText(aText)
  , mType(aType)
  , mTextPalette(palette())
{
    setText(mText);
    mWorkTextPalette.setColor(QPalette::Text, Qt::gray);
    setPalette(mWorkTextPalette);

}

QString	MyLineEdit::text() const
{
    if (mIsEdit)
    {
        return QLineEdit::text();
    }
    return QString();
}

void MyLineEdit::setRealText(QString aText)
{
    if (aText.length())
    {
        setPalette(mTextPalette);
        clear();
        mIsEdit = true;
        setEchoMode(mType);
        setText(aText);
    }
}

void MyLineEdit::focusInEvent(QFocusEvent * e)
{
    if (!mIsEdit)
    {
        setPalette(mTextPalette);
        clear();
        mIsEdit = true;
        setEchoMode(mType);
    }
    QLineEdit::focusInEvent(e);

}

void MyLineEdit::focusOutEvent(QFocusEvent * e)
{
    QLineEdit::focusOutEvent(e);
    if (!text().length())
    {
        setEchoMode(Normal);
        setText(mText);
        mIsEdit = false;
        setPalette(mWorkTextPalette);
    }
}
