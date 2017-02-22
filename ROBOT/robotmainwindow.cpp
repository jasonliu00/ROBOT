#include "robotmainwindow.h"
#include "ui_robotmainwindow.h"
#include "graphicwidget.h"
#include "motorstartdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QIODevice>
#include <QTextStream>

RobotMainWindow::RobotMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RobotMainWindow)
{
//    scene = new DiagramScene(itemMenu, this);
//    scene->setSceneRect(QRectF(0, 0, 5000, 5000));

//    ui->setupUi(this);
    createActions();
    createMenus();
    createToolbars();
    GraphicWidget *widget = new GraphicWidget(this);
    setCentralWidget(widget);
    setWindowTitle(tr("ROBOT"));

}

RobotMainWindow::~RobotMainWindow()
{
    delete ui;
}

void RobotMainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
//    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
//    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
//    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
//    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
//    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    newAction = new QAction(QIcon(":/images/new.png"), tr("(&N)新建"), this);
    newAction->setShortcut(tr("New"));
    newAction->setStatusTip(tr("新建流程图程序"));

    openAction = new QAction(QIcon(":/images/open.png"), tr("(&O)打开"),this);
    openAction->setShortcut(tr("Open"));
    openAction->setStatusTip(tr("打开流程图程序"));

    saveAction = new QAction(QIcon(":/images/save.png"), tr("(&S)保存"),this);
    saveAction->setShortcut(tr("Save"));
    saveAction->setStatusTip(tr("保存流程图程序"));

    compellAction = new QAction(QIcon(":/images/compell.png"), tr("(&C)编译"),this);
    compellAction->setShortcut(tr("Compell"));
    compellAction->setStatusTip(tr("编译当前程序"));
    connect(compellAction, SIGNAL(triggered(bool)), this, SLOT(compellProgram()));

    downloadAction = new QAction(QIcon(":/images/download.png"), tr("(&D)下载"),this);
    downloadAction->setShortcut(tr("Download"));
    downloadAction->setStatusTip(tr("下载程序"));

    motorStartAction = new QAction(QIcon(":/images/MotorTS1.PNG"), tr("马达启动"),this);
    connect(motorStartAction, SIGNAL(triggered()), this, SLOT(showMotorDlg()));
}

//void RobotMainWindow::createToolBox()
//{

//}

void RobotMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&文件"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&编译"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&下载"));
    aboutMenu->addAction(aboutAction);
}

void RobotMainWindow::createToolbars()
{
    toolBar = addToolBar(tr("工具栏"));
    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(compellAction);
    toolBar->addAction(downloadAction);
    toolBar->addAction(motorStartAction);
}

void RobotMainWindow::showMotorDlg()
{
    MotorStartDialog *dlg = new MotorStartDialog();
    dlg->show();
}

void RobotMainWindow::compellProgram()
{
    QSqlQuery query;
    QString out0;
    QFile file("e:/Robotprogram/demo.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "can not open file\n";
        file.close();
        return;
    }
    QTextStream in(&file);
    /***********判断有无开始模块***********/
    if(!query.exec("SELECT out0, content FROM property WHERE name = 'CSH';")){
        qDebug() << "Select CSH module query failed\n"
                 << query.lastError().text();
        file.close();
        return;
    }
    if(!query.next()){
        QMessageBox::warning(this, tr("warning"), tr("缺少开始模块！"));
        file.close();
        return;
    }else if(query.value(0).toString() == NULL){
        QMessageBox::warning(this, tr("warning"), tr("请连接开始模块！"));
        file.close();
        return;
    }else{
        out0 = query.value(0).toString();
        in << "CSH:\n";
        if(query.value(1).toString() == NULL)
            in << "/\n";
        else
            in << query.value(1).toString();
        in << "GOTO:" << out0 << ";\n";
    }
    /********遍历表**********/
    if(!query.exec("SELECT type, name, innum, out0, out1, content FROM property WHERE innum != 0;")){
        qDebug() << "Select innum != 0 query failded\n"
                 << query.lastError().text();
        file.close();
        return;
    }
    while(query.next()){
        /***********判断有无结束模块***********/
        if(query.value(3).toString() == NULL && query.value(0).toString() != "JS"){
            QMessageBox::warning(this, tr("warning"), tr("缺少结束模块，请连接结束模块！"));
            file.close();
            return;
        }
        in << query.value(1).toString() << ":\n";
        /************单独处理判断模块**************/
        if(query.value(0).toString() == "PD"){
            in << "IF " << query.value(5).toString() << "\n";
            in << "GOTO:" << query.value(3).toString() << ";\n";
            if(query.value(4).toString() == NULL)
                in << "ELSE GOTO:/\n";
            else
                in << "ELSE GOTO:" << query.value(4).toString() << ";\n";
        }
        else{
            if(query.value(5).toString() == NULL)
                in << "/\n";
            else
                in << query.value(5).toString() << "\n";
            if(query.value(0).toString() != "JS")
                in << "GOTO:" << query.value(3).toString() << ";\n";
        }
    }

    file.close();
}
