#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *m_ui;

private slots:
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_txtDocument_redoAvailable(bool b);
    void on_txtDocument_undoAvailable(bool b);
};

#endif // MAINWINDOW_H
