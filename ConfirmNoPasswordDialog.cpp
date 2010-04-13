#include "ConfirmNoPasswordDialog.h"
#include "ui_ConfirmNoPasswordDialog.h"

ConfirmNoPasswordDialog::ConfirmNoPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmNoPasswordDialog)
{
    ui->setupUi(this);
}

ConfirmNoPasswordDialog::~ConfirmNoPasswordDialog()
{
    delete ui;
}

void ConfirmNoPasswordDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ConfirmNoPasswordDialog::on_btnSaveWithoutPassword_clicked()
{
    m_returnState = SaveWithoutPassword;
    accept();
}

void ConfirmNoPasswordDialog::on_btnChoosePassword_clicked()
{
    m_returnState = ChoosePassword;
    accept();
}

void ConfirmNoPasswordDialog::on_btnCancel_clicked()
{
    m_returnState = Cancel;
    reject();
}
