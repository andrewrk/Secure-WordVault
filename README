== Setting up your Environment ==

Download and install Qt. It comes with QtCreator
clone the git repository, and open the .pro file with QtCreator
you can use any OS.

== How to Deploy ==
1.  Use a Windows 32-bit operating system
2.  Download and install Qt.
3.  Download and install MinGW for Windows.
4.  Add MinGW bin folder to PATH.
5.  Edit %qtdir%/mkspecs/win32-g++/qmake.conf
    Prefix -static to QMAKE_LFLAGS
6.  cd %qtdir%
7.  configure -static -release -no-exceptions
    this step will take minutes
8.  mingw32-make
    this step will take hours
9.  build the application in qt creator in release mode
10. strip -s securenotepad.exe
11. upx -9 securenotepad.exe
    You can find upx at http://upx.sourceforge.net/

