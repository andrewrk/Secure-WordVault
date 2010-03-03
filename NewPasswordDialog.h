#ifndef PASSWORDINPUTDIALOG_H
#define PASSWORDINPUTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class NewPasswordDialog;
}

class NewPasswordDialog : public QDialog {
    Q_OBJECT
public:
    NewPasswordDialog(QWidget *parent = 0);
    ~NewPasswordDialog();

    // get the password that was selected
    QString password();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::NewPasswordDialog *m_ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // PASSWORDINPUTDIALOG_H
