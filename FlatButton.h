#ifndef FLATBUTTON_H
#define FLATBUTTON_H

#include <QPushButton>
#include <QEvent>

class FlatButton : public QPushButton
{
public:
    FlatButton(QWidget * parent = 0);
    FlatButton(const QString & text, QWidget * parent = 0);
protected:
    void enterEvent(QEvent * e);
    void leaveEvent(QEvent * e);
};

#endif // FLATBUTTON_H
