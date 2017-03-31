#ifndef ROBOTMAINWINDOW_H
#define ROBOTMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "mygraphicsscene.h"

const qint32 MagicNumber = 0x5A93DE5;
const qint16 VersionNumber = 1;
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
    bool okToClearData();
    void loadFile();
    void readItems(QDataStream &in, int offset=0, bool select=false);
    void writeItems(QDataStream &out,
                    const QList<QGraphicsItem*> &items);
    void setDirty(bool on);
    void clear();
    bool openRobotProgramFile(QFile *file, QDataStream &in);
    void saveDB(const QString &path);
    void loadDB(const QString &path);
    void clearDB();
private slots:
    void fileOpen();
    void fileNew();
    bool fileSave();
    bool fileSaveAs();
    void showMotorDlg();
    void compellProgram();
    bool download();
    void about();

private:
    Ui::RobotMainWindow *ui;

//    QAction *exitAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;

    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *openAction;
    QAction *compellAction;
    QAction *downloadAction;

    QAction *motorStartAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *aboutMenu;

    QToolBar *toolBar;

    QString dbFileName;
    QString proFileName;


//    MyGraphicsScene *scene;
//    QGraphicsView *view;
};

#endif // ROBOTMAINWINDOW_H
