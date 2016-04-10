#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argv);
#ifdef QT_DEBUG
    w.show();
#else
    w.showFullScreen();
#endif
    return a.exec();
}
