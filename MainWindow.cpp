#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "NewPasswordDialog.h"
#include "AboutDialog.h"
#include "ChangePasswordDialog.h"
#include "ExeParser.h"
#include "PasswordInputDialog.h"
#include "Encryption.h"
#include "FlatButton.h"

#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QDateTime>
#include <QCheckBox>
#include <QPushButton>
#include <QTextDocumentFragment>
#include <QFontDialog>

#include <cstdlib>

MainWindow::MainWindow(QString targetExe, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_tainted(false),
    m_targetExe(QString()),
    m_txtFind(NULL),
    m_txtFindAction(NULL),
    m_lastSearchFound(true),
    m_findFlags((QTextDocument::FindFlag) 0),
    m_manuallyMovedCursor(true)
{
    m_ui->setupUi(this);

    // tidy up some menus
    m_ui->actionExit->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_F4));
    m_ui->actionAbout->setText(tr("&About %1...").arg(QApplication::applicationName()));

    setCentralWidget(m_ui->txtDocument);

    // set up the search bar
    // close box
    m_ui->findBar->addAction(QIcon(":/icons/WindowClose.png"), QString(), this, SLOT(hideFindBar()));

    // "Find:" label
    QLabel * findLabel = new QLabel(tr("Find:"), m_ui->findBar);
    m_ui->findBar->addWidget(findLabel);

    // text box
    m_txtFind = new QLineEdit(m_ui->findBar);
    m_txtFind->setMaximumWidth(200);
    m_txtFind->setMinimumWidth(50);
    m_defaultBackgroundColor = m_txtFind->palette().color(m_txtFind->backgroundRole());
    connect(m_txtFind, SIGNAL(returnPressed()), this, SLOT(guiFindNext()));
    connect(m_txtFind, SIGNAL(textChanged(QString)), this, SLOT(updateSearch()));
    m_txtFindAction = m_ui->findBar->addWidget(m_txtFind);

    // find next and previous buttons
    m_ui->findBar->addAction(QIcon(":/icons/GoToPrevious.png"), QString(), this, SLOT(guiFindPrevious()));
    m_ui->findBar->addAction(QIcon(":/icons/GoToNextHS.png"), QString(), this, SLOT(guiFindNext()));

    // determine the default hilight palette colors
    QPalette palette = m_ui->txtDocument->palette();
    m_defaultHilightColor = palette.color(QPalette::Highlight);
    m_defaultHilightTextColor = palette.color(QPalette::HighlightedText);

    // jump the gun
    showFindGui();
    m_ui->txtDocument->setFocus(Qt::OtherFocusReason);

    // open the file that was passed in
    if (targetExe.isNull()) {
        guiNew();
    } else {
        if (! guiOpen(targetExe))
            std::exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete m_txtFind;
    delete m_ui;
}

void MainWindow::toggleWholeWordSearch(bool value)
{
    if (value)
        m_findFlags |= QTextDocument::FindWholeWords;
    else
        m_findFlags &= ~QTextDocument::FindWholeWords;
    updateSearch();
}

void MainWindow::toggleCaseSensitiveSearch(bool value)
{
    if (value)
        m_findFlags |= QTextDocument::FindCaseSensitively;
    else
        m_findFlags &= ~QTextDocument::FindCaseSensitively;
    updateSearch();
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Escape && m_ui->findBar->isVisible()) {
        hideFindBar();
    } else if (e->key() == Qt::Key_F3 && e->modifiers() & Qt::ShiftModifier) {
        guiFindPrevious();
    }
}

void MainWindow::hideFindBar()
{
    m_ui->findBar->hide();
    m_ui->txtDocument->setFocus(Qt::OtherFocusReason);

    updateGui();
}

QString MainWindow::boolSerializer(bool bl)
{
    return bl ? QString("true") : QString("false");
}

bool MainWindow::boolDeserializer(QString str)
{
    return str == "true";
}

bool MainWindow::guiOpen(QString targetExe)
{
    QByteArray doc = ExeParser::read(targetExe);
    if (doc.isEmpty()) {
        // new document
        guiNew();
        return true;
    } else {
        // prompt for password and decrypt
        while(true) {
            PasswordInputDialog dialog(targetExe, this);
            if (dialog.exec() == QDialog::Rejected)
                return false; // user cancel
            QString password = dialog.password();

            bool ok;
            QString text = Encryption::decrypted(doc, password, &ok);
            if (ok) {
                m_password = password;

                // Parse out the settings
                QStringList data = text.split("\n");

                // Set font setting
                QString fontString = data.takeFirst();
                QFont font;
                font.fromString(fontString);
                m_ui->txtDocument->setFont(font);

                // Set findbar visibility
                QString findBarState = data.takeFirst();
                if (findBarState == "search") {
                    showFindGui();
                    m_ui->txtDocument->setFocus(Qt::OtherFocusReason);
                } else {
                    hideFindBar();
                }

                // Set wordwrap
                bool wordWrapState = boolDeserializer(data.takeFirst());
                m_ui->actionWordWrap->setChecked(wordWrapState);

                // Set document text
                QString document = data.join("\n");
                m_ui->txtDocument->setPlainText(document);

                break;
            } else {
                QMessageBox::warning(this, QApplication::applicationName(),
                    tr("Sorry, wrong password.\n\n(Make sure your CAPS lock is off and try again.)"));
            }
        }
    }

    m_ui->actionUndo->setEnabled(false);
    m_ui->actionRedo->setEnabled(false);

    m_targetExe = targetExe;
    m_tainted = false;
    updateGui();
    return true;
}

void MainWindow::updateGui()
{
    m_ui->txtDocument->setLineWrapMode(m_ui->actionWordWrap->isChecked() ?
                                       QPlainTextEdit::WidgetWidth :
                                       QPlainTextEdit::NoWrap);
    m_ui->actionChangePassword->setEnabled(! m_password.isNull());

    // color the search text box red if not found
    QPalette pal = m_txtFind->palette();
    if (m_lastSearchFound || m_txtFind->text().isEmpty())
        pal.setColor(m_txtFind->backgroundRole(), m_defaultBackgroundColor);
    else
        pal.setColor(m_txtFind->backgroundRole(), QColor(255, 102, 102));
    m_txtFind->setPalette(pal);

    // color the search hilighted text yellow if found
    pal = m_ui->txtDocument->palette();
    if (m_ui->findBar->isVisible() && ! m_txtFind->text().isEmpty() && ! m_manuallyMovedCursor) {
        pal.setColor(QPalette::Highlight, Qt::yellow);
        pal.setColor(QPalette::HighlightedText, Qt::black);
    } else {
        pal.setColor(QPalette::Highlight, QColor(51, 133, 255)); // if only m_defaultHilightColor would work.
        pal.setColor(QPalette::HighlightedText, Qt::white); // if only m_defaultHilightTextColor would work.
    }
    m_ui->txtDocument->setPalette(pal);

    updateCaption();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_txtDocument_undoAvailable(bool b)
{
    m_ui->actionUndo->setEnabled(b);
}

void MainWindow::on_txtDocument_redoAvailable(bool b)
{
    m_ui->actionRedo->setEnabled(b);
}

void MainWindow::on_actionUndo_triggered()
{
    m_ui->txtDocument->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    m_ui->txtDocument->redo();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (ensureSaved())
        e->accept();
    else
        e->ignore();
}

bool MainWindow::ensureSaved()
{
    if (m_tainted) {
        QMessageBox::StandardButton response = QMessageBox::question(this,
            QApplication::applicationName(),
            tr("You have unsaved changes. Would you like to save?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);
        if (response == QMessageBox::Save)
            return guiSave();
        return response != QMessageBox::Cancel;
    } else {
        return true;
    }
}

bool MainWindow::guiSave()
{
    if (! ensurePassword())
        return false; // user cancelled

    if (! m_tainted)
        return true; // no need to save

    if (m_targetExe.isNull())
        return guiSaveAs(); // need to pick a file

    save();
    return true;
}

void MainWindow::save()
{
    // Prepend settings to document
    QString fontString    = m_ui->txtDocument->font().toString();
    QString findBarState  = QString("\n") + getFindBarState();
    QString wordWrapState = QString("\n") + boolSerializer(m_ui->actionWordWrap->isChecked());
    QString data          = QString("\n") + m_ui->txtDocument->toPlainText();
    QString document      = QString();
    document.append(fontString);
    document.append(findBarState);
    document.append(wordWrapState);
    document.append(data);

    ExeParser::write(m_targetExe, Encryption::encrypted(document, m_password));

    m_tainted = false;
    updateGui();
}

QString MainWindow::getFindBarState()
{
    if(m_ui->findBar->isVisible())
        return "search";
    else
        return "none";
}

bool MainWindow::ensurePassword()
{
    if (m_password.isNull()) {
        // prompt for a new password
        NewPasswordDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            m_password = dialog.password();
            m_tainted = true;
        } else {
            return false; // cancelled save
        }
    }

    return true;
}

bool MainWindow::guiSaveAs()
{
    if (! ensurePassword())
        return false; // user cancelled

    // prompt for a location to save the new file
    QString file = QFileDialog::getSaveFileName(this, tr("Save As"),
        QFileInfo(m_targetExe).absoluteDir().path(), tr("Secure WordVault Document (*.exe)"));
    if (file.isNull())
        return false; // pressed cancel
    m_targetExe = file;


    save();
    return true;
}

void MainWindow::on_txtDocument_textChanged()
{
    taint();
}

void MainWindow::taint()
{
    m_tainted = true;
    updateCaption();
}

void MainWindow::updateCaption()
{
    QString tainted = m_tainted ? "*" : "";

    QString title;
    if (m_targetExe.isNull())
        title = tr("Unsaved");
    else
        title = QFileInfo(m_targetExe).completeBaseName();

    this->setWindowTitle(title + tainted + QString(" - ") + QApplication::applicationName());
}

void MainWindow::on_actionSave_triggered()
{
    guiSave();
}

void MainWindow::on_actionSaveAs_triggered()
{
    guiSaveAs();
}

bool MainWindow::guiNew()
{
    if (! ensureSaved())
        return false;

    m_ui->actionUndo->setEnabled(false);
    m_ui->actionRedo->setEnabled(false);

    m_targetExe = QString();
    m_ui->txtDocument->setPlainText("");
    m_tainted = false;
    hideFindBar();
    updateGui();
    return true;
}

void MainWindow::on_actionNew_triggered()
{
    guiNew();
}

void MainWindow::on_actionWordWrap_toggled(bool )
{
    taint();
    updateGui();
}

void MainWindow::on_actionSelectAll_triggered()
{
    m_ui->txtDocument->selectAll();
}

void MainWindow::on_actionDelete_triggered()
{
    m_ui->txtDocument->textCursor().deleteChar();
}

void MainWindow::on_actionCut_triggered()
{
    m_ui->txtDocument->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    m_ui->txtDocument->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    m_ui->txtDocument->paste();
}

void MainWindow::on_actionInsertDateTime_triggered()
{
    m_ui->txtDocument->textCursor().insertText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog(this).exec();
}

void MainWindow::on_actionFAQ_triggered()
{
    QUrl url = QUrl::fromEncoded("http://bluepandatech.com/software/secure-wordvault#faq");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionInstructions_triggered()
{
    QUrl url = QUrl::fromEncoded("http://bluepandatech.com/software/secure-wordvault#instructions");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionChangePassword_triggered()
{
    ChangePasswordDialog dialog(m_password, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_password = dialog.password();
        if (guiSave()) {
            QMessageBox::information(this, QApplication::applicationName(),
                tr("Password changed successfully."), QMessageBox::Ok);
        }
    }
}

void MainWindow::on_actionFind_triggered()
{
    showFindGui();
}

void MainWindow::showFindGui()
{
    m_ui->findBar->show();

    if (m_ui->txtDocument->textCursor().selectionStart() != m_ui->txtDocument->textCursor().selectionEnd())
        m_txtFind->setText(m_ui->txtDocument->textCursor().selection().toPlainText());

    m_txtFind->setFocus(Qt::OtherFocusReason);
    if(m_txtFind->text().size() != 0)
        m_txtFind->selectAll();

    updateGui();
}

void MainWindow::on_actionFindNext_triggered()
{
    guiFindNext();
}

void MainWindow::guiFindNext()
{
    findText((QTextDocument::FindFlag) m_findFlags);
}

void MainWindow::guiFindPrevious()
{
    findText((QTextDocument::FindFlag) (m_findFlags | QTextDocument::FindBackward));
}

void MainWindow::findText(QTextDocument::FindFlag flags)
{
    QTextCursor cursor = m_ui->txtDocument->textCursor();
    int pos = cursor.position();
    bool found = m_ui->txtDocument->find(m_txtFind->text(), flags);
    if (! found) {
        // wrap search
        cursor = m_ui->txtDocument->textCursor();

        if (flags & QTextDocument::FindBackward)
            cursor.setPosition(m_ui->txtDocument->toPlainText().size(), QTextCursor::MoveAnchor);
        else
            cursor.setPosition(0, QTextCursor::MoveAnchor);

        m_ui->txtDocument->setTextCursor(cursor);
        found = m_ui->txtDocument->find(m_txtFind->text(), flags);
    }

    if (! found) {
        cursor = m_ui->txtDocument->textCursor();
        cursor.setPosition(pos, QTextCursor::MoveAnchor);
        m_ui->txtDocument->setTextCursor(cursor);
    }

    m_lastSearchFound = found;
    m_manuallyMovedCursor = false;
    updateGui();
}

void MainWindow::updateSearch()
{
    // move cursor to beginning of current match
    QTextCursor cursor = m_ui->txtDocument->textCursor();
    int lower = cursor.anchor() < cursor.position() ? cursor.anchor() : cursor.position();
    cursor.setPosition(lower, QTextCursor::MoveAnchor);
    m_ui->txtDocument->setTextCursor(cursor);

    guiFindNext();
}

void MainWindow::on_actionFont_triggered()
{
    m_ui->txtDocument->setFont(QFontDialog::getFont(NULL, m_ui->txtDocument->font(), this));
    taint();
}

void MainWindow::on_txtDocument_cursorPositionChanged()
{
    m_manuallyMovedCursor = true;
    updateGui();
}

void MainWindow::on_txtDocument_selectionChanged()
{
    m_manuallyMovedCursor = true;
    updateGui();
}
