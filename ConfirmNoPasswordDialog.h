#ifndef CONFIRMNOPASSWORDDIALOG_H
#define CONFIRMNOPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class ConfirmNoPasswordDialog;
}

class ConfirmNoPasswordDialog : public QDialog {
    Q_OBJECT
public:
    ConfirmNoPasswordDialog(QWidget *parent = 0);
    ~ConfirmNoPasswordDialog();

    enum ReturnState {
        SaveWithoutPassword,
        ChoosePassword,
        Cancel,
    };

    ReturnState returnState() { return m_returnState; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConfirmNoPasswordDialog *ui;

    ReturnState m_returnState;

private slots:
    void on_btnCancel_clicked();
    void on_btnChoosePassword_clicked();
    void on_btnSaveWithoutPassword_clicked();
};

#endif // CONFIRMNOPASSWORDDIALOG_H
