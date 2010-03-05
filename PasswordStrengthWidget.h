#ifndef PASSWORDSTRENGTHWIDGET_H
#define PASSWORDSTRENGTHWIDGET_H

#include <QWidget>
#include <QLineEdit>

class PasswordStrengthWidget : public QWidget
{
    Q_OBJECT
public:
    PasswordStrengthWidget(QWidget * parent = 0);

    // attach to a QLineEdit to provide password strength meter for
    void attachTo(QLineEdit * buddy);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void textChanged(QString);

private: //variables
    float m_strength;

private: //methods
    // calculate the password strength and store it in m_strength
    void calculateStrength(QString password);

    // determine if a string contains any of the characters in characterSet
    bool containsAny(QString text, QString characterSet);
    int containsCount(QString text, QString characterSet);
};

#endif // PASSWORDSTRENGTHWIDGET_H
