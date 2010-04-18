#include "PasswordStrengthWidget.h"

#include <QPainter>

#include <cmath>

PasswordStrengthWidget::PasswordStrengthWidget(QWidget * parent) :
    QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumHeight(QLineEdit().minimumHeight());
}

void PasswordStrengthWidget::attachTo(QLineEdit *buddy)
{
    disconnect(this, SLOT(textChanged(QString)));
    connect(buddy, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    textChanged(buddy->text());
}

void PasswordStrengthWidget::textChanged(QString password)
{
    calculateStrength(password);
    this->update();
}

void PasswordStrengthWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setBackground(Qt::white);
    p.eraseRect(0, 0, width(), height());

    // pick a color and text based on strength
    QList<QColor> colors;
    QStringList names;
    colors.append(Qt::red);
    names.append(tr("Very Weak"));

    colors.append(QColor(255, 167, 20));
    names.append(tr("Weak"));

    colors.append(Qt::yellow);
    names.append(tr("Good"));

    colors.append(QColor(141, 228, 40));
    names.append(tr("Strong"));

    colors.append(QColor(0, 162, 49));
    names.append(tr("Very Strong"));

    int index = (int) (m_strength * colors.size());
    if (index >= colors.size())
        index = colors.size()-1;

    // make sure the bar goes all the way to the right for the strongest password
    if (index == colors.size() - 1) {
        m_strength = 1.0f;
        p.setPen(QPen(Qt::white, 1));
    } else {
        p.setPen(QPen(Qt::black, 1));
    }

    p.setBrush(colors.at(index));
    p.fillRect(0, 0, (int) (m_strength * width()), height(), QBrush(colors.at(index), Qt::SolidPattern));
    p.drawText(0, 0, width(), height(), Qt::AlignCenter, names.at(index));
}

void PasswordStrengthWidget::calculateStrength(QString password)
{
    QString digits = "0123456789";
    QString lowers = "abcdefghijklmnopqrstuvwxyz";
    QString uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString symbols = ";:-_=+\\|/?^&!.@$Â£#*()%`~<>{}[] ";

    bool hasDigits = containsAny(password, digits);
    bool hasLower = containsAny(password, lowers);
    bool hasUpper = containsAny(password, uppers);
    bool hasSymbols = containsAny(password, symbols);
    bool hasMixed = hasLower && hasUpper;
    bool hasLetters = hasLower || hasUpper;

    // 100 based scoring system
    int score = 0;

    // password length
    if (password.size() >= 8)
        score += 25;
    else if(password.size() >= 5)
        score += 10;
    else
        score += 5;

    // letters
    if (hasMixed)
        score += 20;
    else if(hasLetters)
        score += 10;
    else
        score += 0;

    // numbers
    if (containsCount(password, digits) >= 3)
        score += 20;
    else if(hasDigits)
        score += 10;
    else
        score += 0;

    // symbols
    if (containsCount(password, symbols) > 1)
        score += 25;
    else if (hasSymbols)
        score += 10;
    else
        score += 0;

    // bonus
    if (hasMixed && hasDigits && hasSymbols)
        score += 5;
    else if(hasLetters && hasDigits && hasSymbols)
        score += 3;
    else if(hasLetters && hasDigits)
        score += 2;

    m_strength = score / 100.0f;
}

bool PasswordStrengthWidget::containsAny(QString text, QString characterSet)
{
    for (int i=0; i<characterSet.size(); ++i) {
        if (text.contains(characterSet.at(i), Qt::CaseSensitive))
            return true;
    }
    return false;
}

int PasswordStrengthWidget::containsCount(QString text, QString characterSet)
{
    int count = 0;
    for (int i=0; i<characterSet.size(); ++i) {
        if (text.contains(characterSet.at(i), Qt::CaseSensitive))
            ++count;
    }
    return count;
}
