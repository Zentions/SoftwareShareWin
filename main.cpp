#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow win;

    win.move((a.desktop()->width() - win.width()) / 2, (a.desktop()->height() - win.height()) / 2);

    win.show();

    return a.exec();
}
