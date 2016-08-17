#include "biomainwindow.h"
#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BIOMainWindow w;
    w.show();

    return a.exec();
}
