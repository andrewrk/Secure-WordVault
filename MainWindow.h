#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QString targetExe, QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent * e);
    void closeEvent(QCloseEvent * e);

private: //variables
    Ui::MainWindow * m_ui;

    // true if we have unsaved changes
    bool m_tainted;

    // absolute path to the EXE the user clicked to run (the one to save
    // the document into)
    QString m_targetExe;

    // the user's password to the current document in plain text. Only ever
    // stored in memory and while the application is open
    QString m_password;

private: //methods
    // prompts the user to save if necessary. Returns true if they did not
    // press Cancel
    bool ensureSaved();

    // saves the current document, or prompts for password if does not exist yet.
    // returns true if the user did not cancel
    bool guiSave();

    // prompts the user for a new path and saves a new file
    // returns true if they didn't cancel
    bool guiSaveAs();

    // make sure the title bar always displays the correct thing
    void updateCaption();

    // if we don't have a password, prompt the user for one.
    // returns true if they didn't cancel
    bool ensurePassword();

    // actually does the save mechanism
    void save();


private slots:
    void on_actionNew_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSave_triggered();
    void on_txtDocument_textChanged();
    void on_actionExit_triggered();
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_txtDocument_redoAvailable(bool b);
    void on_txtDocument_undoAvailable(bool b);
};

#endif // MAINWINDOW_H
