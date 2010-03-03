#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog {
    Q_OBJECT
public:
    ChangePasswordDialog(QString oldPassword, QWidget *parent = 0);
    ~ChangePasswordDialog();

    QString password();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChangePasswordDialog * m_ui;
    QString m_oldPassword;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // CHANGEPASSWORDDIALOG_H
