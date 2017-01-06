#include "robotmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RobotMainWindow w;
    w.setWindowIcon(QIcon(":/images/title"));
    w.show();

    return a.exec();
}
