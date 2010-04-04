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
    FlatButton.cpp
HEADERS += MainWindow.h \
    ExeParser.h \
    NewPasswordDialog.h \
    AboutDialog.h \
    ChangePasswordDialog.h \
    PasswordInputDialog.h \
    Encryption.h \
    PasswordStrengthWidget.h \
    FlatButton.h \
    ../../../../MinGW/include/openssl/evp.h \
    ../../../../MinGW/include/openssl/md5.h
FORMS += MainWindow.ui \
    NewPasswordDialog.ui \
    AboutDialog.ui \
    ChangePasswordDialog.ui \
    PasswordInputDialog.ui
RESOURCES += resources.qrc
LIBS += "C:\MinGW\lib\libeay32.dll.a"
