#include "PasswordInputDialog.h"
#include "ui_PasswordInputDialog.h"

#include <QMessageBox>

PasswordInputDialog::PasswordInputDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PasswordInputDialog)
{
    m_ui->setupUi(this);
}

PasswordInputDialog::~PasswordInputDialog()
{
    delete m_ui;
}

void PasswordInputDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PasswordInputDialog::on_buttonBox_accepted()
{
    // make sure the passwords match
    if (m_ui->txtPassword->text() != m_ui->txtConfirm->text()) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("Passwords do not match."),
            QMessageBox::Ok);
        // clear both boxes and set focus to first
        m_ui->txtConfirm->setText(QString());
        m_ui->txtPassword->setText(QString());
        m_ui->txtPassword->setFocus(Qt::OtherFocusReason);
    } else {
        accept();
    }
}

void PasswordInputDialog::on_buttonBox_rejected()
{
    reject();
}

QString PasswordInputDialog::password()
{
    return m_ui->txtPassword->text();
}
