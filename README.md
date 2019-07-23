# kptc
Kptc is a Qt application for operating comfortable with the SCS PTC-II ( or PTC-IIe, PTC-IIpro)

## Build kptc
Kptc depends on Qt 5, the needed modules are core, gui and widgets. Recently the minimal Qt version required for build the project has shift to Qt 5.11. It is assumed that you can compile with C++14. If you can not do this, you can remove this setting from kptc.pro file. Kptc is going to depend on C++14 in future versions.

The following way should work:

qmake . 

make 

Maybe the qmake binary has another name (e. g. under openSUSE qmake-qt5).

But this does not work currently with openSUSE (I do not know why). But to build kptc there, run build from QtCreator, that works fine.

Please note that is a beta version.
