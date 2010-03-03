#include "PasswordInputDialog.h"
#include "ui_PasswordInputDialog.h"

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
    accept();
}

void PasswordInputDialog::on_buttonBox_rejected()
{
    reject();
}

QString PasswordInputDialog::password()
{
    return m_ui->txtPassword->text();
}
