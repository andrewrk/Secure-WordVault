#include "PasswordInputDialog.h"
#include "ui_PasswordInputDialog.h"

#include <QFileInfo>

PasswordInputDialog::PasswordInputDialog(QString filename, QWidget *parent) :
    QDialog(),
    m_ui(new Ui::PasswordInputDialog)
{
    m_ui->setupUi(this);
    //m_ui->lblFile->setText(tr("File: ") + QFileInfo(filename).fileName());
    m_ui->lblFileName->setText(QFileInfo(filename).fileName());

    QIcon icon = windowIcon();
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    setWindowFlags(flags);
    setWindowIcon(icon);
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
