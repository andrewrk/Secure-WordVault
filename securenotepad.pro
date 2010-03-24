# -------------------------------------------------
# Project created by QtCreator 2010-02-25T18:07:41
# -------------------------------------------------
TARGET = securenotepad
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    ExeParser.cpp \
    NewPasswordDialog.cpp \
    AboutDialog.cpp \
    ChangePasswordDialog.cpp \
    PasswordInputDialog.cpp \
    Encryption.cpp \
    PasswordStrengthWidget.cpp \
    FlatButton.cpp \
    aestab.c \
    aeskey.c \
    aescrypt.c \
    aes_modes.c
HEADERS += MainWindow.h \
    ExeParser.h \
    NewPasswordDialog.h \
    AboutDialog.h \
    ChangePasswordDialog.h \
    PasswordInputDialog.h \
    Encryption.h \
    PasswordStrengthWidget.h \
    FlatButton.h \
    brg_types.h \
    brg_endian.h \
    aestab.h \
    aesopt.h \
    aescpp.h \
    aes.h \
    aes_via_ace.h
FORMS += MainWindow.ui \
    NewPasswordDialog.ui \
    AboutDialog.ui \
    ChangePasswordDialog.ui \
    PasswordInputDialog.ui
RESOURCES += resources.qrc
