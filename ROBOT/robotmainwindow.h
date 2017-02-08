#ifndef ROBOTMAINWINDOW_H
#define ROBOTMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "mygraphicsscene.h"

namespace Ui {
class RobotMainWindow;
}

class RobotMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotMainWindow(QWidget *parent = 0);
    ~RobotMainWindow();

private:
//    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();


private:
    Ui::RobotMainWindow *ui;

    QAction *exitAction;
//    QAction *addAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;

    QAction *newAction;
    QAction *saveAction;
    QAction *openAction;
    QAction *compellAction;
    QAction *downloadAction;

    QAction *motorStartAction;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;

    QToolBar *toolBar;

private slots:
    void showMotorDlg();
    void compellProgram();


//    MyGraphicsScene *scene;
//    QGraphicsView *view;
};

#endif // ROBOTMAINWINDOW_H
