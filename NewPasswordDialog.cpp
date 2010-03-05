#include "NewPasswordDialog.h"
#include "ui_NewPasswordDialog.h"

#include <QMessageBox>

NewPasswordDialog::NewPasswordDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::NewPasswordDialog)
{
    m_ui->setupUi(this);
    m_ui->strengthMeter->attachTo(m_ui->txtPassword);
}

NewPasswordDialog::~NewPasswordDialog()
{
    delete m_ui;
}

void NewPasswordDialog::changeEvent(QEvent *e)
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

void NewPasswordDialog::on_buttonBox_accepted()
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

void NewPasswordDialog::on_buttonBox_rejected()
{
    reject();
}

QString NewPasswordDialog::password()
{
    return m_ui->txtPassword->text();
}
