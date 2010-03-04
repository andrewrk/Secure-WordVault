#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QString targetExe = QString(), QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent * e);
    void closeEvent(QCloseEvent * e);
    void keyPressEvent(QKeyEvent * e);

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

    // the find text box in the status bar
    QLineEdit * m_txtFind;

    // true if the last search the user did found something
    bool m_lastSearchFound;

    // the normal background color of a QLineEdit
    QColor m_defaultBackgroundColor;

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

    // opens an executable file and set as current document
    // returns true if they didn't cancel
    bool guiOpen(QString targetExe);

    // unassociates the target exe and clears the document
    // returns true if they didn't cancel
    bool guiNew();

    // make sure the title bar always displays the correct thing
    void updateCaption();

    // make sure the correct buttons and things are shown and enabled
    void updateGui();

    // if we don't have a password, prompt the user for one.
    // returns true if they didn't cancel
    bool ensurePassword();

    // actually does the save mechanism
    void save();

private slots:
    void on_actionFindNext_triggered();
    void on_actionFind_triggered();
    void on_actionChangePassword_triggered();
    void on_actionAbout_triggered();
    void on_actionInsertDateTime_triggered();
    void on_actionPaste_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionDelete_triggered();
    void on_actionSelectAll_triggered();
    void on_actionWordWrap_toggled(bool );
    void on_actionNew_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSave_triggered();
    void on_txtDocument_textChanged();
    void on_actionExit_triggered();
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_txtDocument_redoAvailable(bool b);
    void on_txtDocument_undoAvailable(bool b);

    // select and scroll to the next instance of the search text
    void guiFindNext();
    void guiFindPrevious();

    // respond to a change in the search text. If the current match can be
    // expanded, do it. otherwise find next.
    void updateSearch();

};

#endif // MAINWINDOW_H
