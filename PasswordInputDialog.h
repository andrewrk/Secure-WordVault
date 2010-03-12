#ifndef PASSWORDINPUTDIALOG_H
#define PASSWORDINPUTDIALOG_H

#include <QDialog>

namespace Ui {
    class PasswordInputDialog;
}

class PasswordInputDialog : public QDialog {
    Q_OBJECT
public:
    PasswordInputDialog(QString filename, QWidget *parent = 0);
    ~PasswordInputDialog();

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
