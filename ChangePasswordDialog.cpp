#include "ChangePasswordDialog.h"
#include "ui_ChangePasswordDialog.h"

#include "ConfirmNoPasswordDialog.h"

#include <QMessageBox>

ChangePasswordDialog::ChangePasswordDialog(QString oldPassword, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ChangePasswordDialog),
    m_oldPassword(oldPassword)
{
    m_ui->setupUi(this);
    m_ui->strengthMeter->attachTo(m_ui->txtNewPassword);

    QIcon icon = windowIcon();
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    setWindowFlags(flags);
    setWindowIcon(icon);
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete m_ui;
}

void ChangePasswordDialog::changeEvent(QEvent *e)
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

void ChangePasswordDialog::on_buttonBox_accepted()
{
    // check if password is blank
    if (m_ui->txtNewPassword->text().isEmpty()) {
        ConfirmNoPasswordDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            if (dialog.returnState() == ConfirmNoPasswordDialog::ChoosePassword) {
                m_ui->txtNewPassword->setFocus(Qt::OtherFocusReason);
                return;
            } else if (dialog.returnState() == ConfirmNoPasswordDialog::SaveWithoutPassword) {
                // take no action here
            }
        } else {
            reject();
            return;
        }
    }

    // make sure old password matches
    if (m_oldPassword != m_ui->txtOldPassword->text()) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("Old password is incorrect."),
            QMessageBox::Ok);
        // clear both boxes and set focus to first
        m_ui->txtOldPassword->setText(QString());
        m_ui->txtOldPassword->setFocus(Qt::OtherFocusReason);
        return;
    }

    // make sure confirm password is same
    if (m_ui->txtNewPassword->text() != m_ui->txtConfirmNewPassword->text()) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("Passwords do not match."),
            QMessageBox::Ok);
        // clear both boxes and set focus to first
        m_ui->txtNewPassword->setText(QString());
        m_ui->txtConfirmNewPassword->setText(QString());
        m_ui->txtNewPassword->setFocus(Qt::OtherFocusReason);
        return;
    }

    accept();
}

void ChangePasswordDialog::on_buttonBox_rejected()
{
    reject();
}

QString ChangePasswordDialog::password()
{
    return m_ui->txtNewPassword->text();
}
