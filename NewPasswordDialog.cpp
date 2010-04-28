#include "NewPasswordDialog.h"
#include "ui_NewPasswordDialog.h"

#include "ConfirmNoPasswordDialog.h"
#include <QMessageBox>

NewPasswordDialog::NewPasswordDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::NewPasswordDialog)
{
    m_ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    setWindowFlags(flags);
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
    // check if password is blank
    if (m_ui->txtPassword->text().isEmpty()) {
        ConfirmNoPasswordDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            if (dialog.returnState() == ConfirmNoPasswordDialog::ChoosePassword) {
                m_ui->txtPassword->setFocus(Qt::OtherFocusReason);
                return;
            } else if (dialog.returnState() == ConfirmNoPasswordDialog::SaveWithoutPassword) {
                // take no action here
            }
        } else {
            reject();
            return;
        }
    }

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
