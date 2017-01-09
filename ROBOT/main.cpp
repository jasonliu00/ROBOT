#include "robotmainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("robot");
    db.setUserName("liulizhao");
    db.setPassword("910228");
    if(!db.open()){
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection())
        return 1;
    RobotMainWindow w;
    w.setWindowIcon(QIcon(":/images/title"));
    w.show();

    return a.exec();
}
