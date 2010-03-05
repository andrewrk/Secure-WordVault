#include "FlatButton.h"

FlatButton::FlatButton(QWidget *parent) :
    QPushButton(parent)
{
    setFlat(true);
}

FlatButton::FlatButton(const QString & text, QWidget *parent) :
    QPushButton(text, parent)
{
    setFlat(true);
}

void FlatButton::enterEvent(QEvent *e)
{
    setFlat(false);
}

void FlatButton::leaveEvent(QEvent *e)
{
    setFlat(true);
}
