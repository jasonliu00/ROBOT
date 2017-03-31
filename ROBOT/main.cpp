#include "robotmainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("robot");
    db.setUserName("robotdb");
    db.setPassword("000000");
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
    //为移植mysql程序所用
    QString strLibPath(QDir::toNativeSeparators(QApplication::applicationDirPath())
                       +QDir::separator()+"plugins");
    QCoreApplication::addLibraryPath(strLibPath);

    if(!createConnection())
        return 1;
    RobotMainWindow w;
    w.setWindowIcon(QIcon(":/images/VC1.0.png"));
    w.show();

    return a.exec();
}

