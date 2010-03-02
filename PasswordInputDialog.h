#ifndef PASSWORDINPUTDIALOG_H
#define PASSWORDINPUTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class PasswordInputDialog;
}

class PasswordInputDialog : public QDialog {
    Q_OBJECT
public:
    PasswordInputDialog(QWidget *parent = 0);
    ~PasswordInputDialog();

    // get the password that was selected
    QString password();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::PasswordInputDialog *m_ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // PASSWORDINPUTDIALOG_H
