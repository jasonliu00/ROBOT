#include "robotmainwindow.h"
#include "ui_robotmainwindow.h"
#include "graphicwidget.h"
#include "motorstartdialog.h"
#include "mygraphicsscene.h"
#include "graphicwidget.h"
#include "arrow.h"
#include "windows.h"   //GetVolumeInformation 函数依赖的头文件
#include "winbase.h"   //GetVolumeInformation 函数在win7下需要的头文件
#include "fileapi.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QIODevice>
#include <QTextStream>
#include <QSqlRecord>
#include <QSqlField>

const int MSecPerSecond = 1000;
const int StatusTimeout = MSecPerSecond * 30;
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
//    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
//                                tr("Bring to &Front"), this);
//    toFrontAction->setShortcut(tr("Ctrl+F"));
//    toFrontAction->setStatusTip(tr("Bring item to front"));
//    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

//    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
//    sendBackAction->setShortcut(tr("Ctrl+T"));
//    sendBackAction->setStatusTip(tr("Send item to back"));
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
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    newAction = new QAction(QIcon(":/images/new.png"), tr("(&N)新建"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("新建流程图程序"));
    connect(newAction, SIGNAL(triggered(bool)), this, SLOT(fileNew()));

    openAction = new QAction(QIcon(":/images/open.png"), tr("(&O)打开"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("打开流程图程序"));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(fileOpen()));

    saveAction = new QAction(QIcon(":/images/save.png"), tr("(&S)保存"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("保存流程图程序"));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(fileSave()));

    saveAsAction = new QAction(QIcon(":/images/saveas.png"), tr("(&A)另存为"), this);
    saveAsAction->setShortcut(tr("SaveAs"));
    saveAsAction->setStatusTip(tr("流程图另存为"));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(fileSaveAs()));

    compellAction = new QAction(QIcon(":/images/compell.png"), tr("(&C)编译"),this);
    compellAction->setShortcut(tr("Ctrl+C"));
    compellAction->setStatusTip(tr("编译当前程序"));
    connect(compellAction, SIGNAL(triggered(bool)), this, SLOT(compellProgram()));

    downloadAction = new QAction(QIcon(":/images/download.png"), tr("(&D)下载"),this);
    downloadAction->setShortcut(tr("Ctrl+D"));
    downloadAction->setStatusTip(tr("下载程序"));
    connect(downloadAction, SIGNAL(triggered(bool)), this, SLOT(download()));
}

//void RobotMainWindow::createToolBox()
//{

//}

void RobotMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&文件"));
    fileMenu->addAction(exitAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    editMenu = menuBar()->addMenu(tr("&编辑"));
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(compellAction);
    editMenu->addAction(downloadAction);

    aboutMenu = menuBar()->addMenu(tr("&关于"));
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
//    toolBar->addAction(motorStartAction);
}

bool RobotMainWindow::okToClearData()
{
//    if (isWindowModified())
//        return AQP::okToClearData(&MainWindow::fileSave, this,
//                tr("Unsaved changes"), tr("Save unsaved changes?"));
    return true;
}

void RobotMainWindow::loadFile()
{
    QFile file(windowFilePath());
    QDataStream in;
    if (!openRobotProgramFile(&file, in))
        return;
    in.setVersion(QDataStream::Qt_5_7);
    clear();
    readItems(in);
    statusBar()->showMessage(tr("Loaded %1").arg(windowFilePath()),
                             StatusTimeout);
    clearDB();
    setDirty(false);
}

void RobotMainWindow::readItems(QDataStream &in, int offset, bool select)
{
    Q_UNUSED(offset);
    Q_UNUSED(select);
    qint32 itemType;
    QMap<QString, ModelGraphicsItem*> map;
    QVector<QString> namevector;
    QVector<QPointF> pointvector;
    GraphicWidget *centralwidget = static_cast<GraphicWidget *>(this->centralWidget());
    MyGraphicsScene *scene = static_cast<MyGraphicsScene*>(centralwidget->getScene());
    QMenu *itemmenu = centralwidget->getItemMenu();
    while(!in.atEnd()){
        in >> itemType;
        switch(itemType){
            case MyCGQItemType:{
                int type;
                in >> type;
                MyCGQItem::CGQType cgqtype = MyCGQItem::CGQType(type);
                MyCGQItem *cgqitem = new MyCGQItem(itemmenu, cgqtype);
                in >> *cgqitem;
                scene->addItem(cgqitem);
                break;
            }
            case MyKZQItemType:{
                int type;
                in >> type;
                MyKZQItem::KZQType kzqtype = MyKZQItem::KZQType(type);
                MyKZQItem *kzqitem = new MyKZQItem(itemmenu, kzqtype);
                in >> *kzqitem;
                scene->addItem(kzqitem);
                map.insert(kzqitem->getName(), kzqitem);
                break;
            }
            case MyZXQItemType:{
                int type;
                in >> type;
                MyZXQItem::ZXQType zxqtype = MyZXQItem::ZXQType(type);
                MyZXQItem *zxqitem = new MyZXQItem(itemmenu, zxqtype);
                in >> *zxqitem;
                scene->addItem(zxqitem);
                map.insert(zxqitem->getName(), zxqitem);
                break;
            }
            case ArrowItemType:{
                QString startItemName, endItemName;
                QPointF startpoint, endpoint;
                in >> startItemName >> endItemName
                   >> startpoint >> endpoint;
                namevector.append(startItemName);
                namevector.append(endItemName);
                pointvector.append(startpoint);
                pointvector.append(endpoint);
                break;
            }
            default:
                break;
        }
        //        if (item) {
        //            item->moveBy(offset, offset);
        //            if (select)
        //                items << item;
        //            item = 0;
        //        }
    }
    for(int i = 0; i < namevector.count(); i+=2){
        ModelGraphicsItem *startitem = map.value(namevector.at(i));
        ModelGraphicsItem *enditem = map.value(namevector.at(i+1));
        Arrow *arrow = new Arrow(startitem, enditem,
                                 pointvector.at(i), pointvector.at(i+1));
        startitem->addArrow(arrow);
        enditem->addArrow(arrow);
        arrow->setZValue(-1000.0);
        scene->addItem(arrow);
        arrow->updatePosition();
    }
//    if (select)
//        selectItems(items);
//    else
//        selectionChanged();
}

void RobotMainWindow::writeItems(QDataStream &out, const QList<QGraphicsItem *> &items)
{
    foreach (QGraphicsItem *item, items) {
        qint32 type = static_cast<qint32>(item->type());
        out << type;
        switch (type) {
            case MyCGQItemType:
                    out << (int)static_cast<MyCGQItem*>(item)->cgqType();
                    out << *static_cast<MyCGQItem*>(item);
            break;
            case MyZXQItemType:
                    out << (int)static_cast<MyZXQItem*>(item)->zxqType();
                    out << *static_cast<MyZXQItem*>(item);
            break;
            case MyKZQItemType:
                    out << (int)static_cast<MyKZQItem*>(item)->kzqType();
                    out << *static_cast<MyKZQItem*>(item);
            break;
//            default: Q_ASSERT(false);
            case ArrowItemType:
                    out << static_cast<Arrow*>(item)->getStartItemName()
                        << static_cast<Arrow*>(item)->getEndItemName()
                        << static_cast<Arrow*>(item)->getStartPoint()
                        << static_cast<Arrow*>(item)->getEndPoint();
            break;
            default:
            break;
        }
    }
}

void RobotMainWindow::setDirty(bool on)
{
    setWindowModified(on);
}

void RobotMainWindow::clear()
{
    ((GraphicWidget *)centralWidget())->getScene()->clear();
    clearDB();
}

bool RobotMainWindow::openRobotProgramFile(QFile *file, QDataStream &in)
{
    if (!file->open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file: %1")
                                        .arg(file->errorString()));
        return false;
    }
    in.setDevice(file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber) {
        QMessageBox::warning(this, tr("Error"),
                tr("%1 is not a %2 file").arg(file->fileName())
                .arg(QApplication::applicationName()));
        return false;
    }
    qint16 versionNumber;
    in >> versionNumber;
    if (versionNumber > VersionNumber) {
        QMessageBox::warning(this, tr("Error"),
                tr("%1 needs a more recent version of %2")
                .arg(file->fileName())
                .arg(QApplication::applicationName()));
        return false;
    }
    return true;
}
/**
 *@brief 导出数据库数据到文件中
 *@param path 文件路径
 */
void RobotMainWindow::saveDB(const QString &path)
{
    QStringList vList;

    // 导出数据库所有名称以auth开头的表
    QStringList tables;
    tables.append("property");
    tables.append("sensorvariable");
    foreach(QString table,tables)
    {
        QSqlQuery query;
        QString sql = QString("SELECT * FROM %1").arg(table);
        query.exec(sql);

        QSqlRecord record=query.record();
        while(query.next())
        {
            QString prefix=QString("INSERT INTO %1(").arg(table); // 记录属性字段名
            QString suffix="VALUES("; // 记录属性值

            // 遍历属性字段
            for(int i=0;i<record.count();i++)
            {
                QSqlField field=record.field(i);
                QString fieldName=field.name();

                switch(field.type())
                {
                case QVariant::String:
                    prefix+=fieldName;
                    suffix+=QString("'%1'").arg(query.value(i).toString().remove('\n'));
                    break;
                case QVariant::Int:
                    {
                        prefix += fieldName;
                        suffix += query.value(i).toInt();
//                        int data=query.value(i).toInt();
//                        if(data.isNull())
//                        {
//                            suffix+="null";
//                        }else
//                        {
//                            suffix+=QString("E'%1'").arg(data.toHex().data()); // blob数据按16进制格式导出
//                        }
                    }
                    break;
                default:
                    prefix+=fieldName;
                    suffix+=query.value(i).toString().remove('\n');
                }

                if(record.count()==1)
                {
                    prefix+=")";
                    suffix+=")";
                }else if(i!=record.count()-1)
                {
                    prefix+=",";
                    suffix+=",";
                }else if(i==record.count()-1)
                {
                    prefix+=")";
                    suffix+=")";
                }
            }

            // 组装sql语句 insert into auth_test values(0,'hello',E'003f')
            QString iSql=QString("%1 %2;").arg(prefix).arg(suffix);
            vList.append(iSql);
        }
    }

    QFile file(path);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);

    // 将sql语句写入文件
    QTextStream out(&file);
    foreach(QString line,vList)
    {
        out<<line+"\n";
    }
}
/**
 *@brief 读取sql文本内容,并写入至数据库
 *@param path sql文件路径
 */
void RobotMainWindow::loadDB(const QString &path)
{
    QSqlQuery query;
    QFile file(path);
    file.open(QFile::ReadOnly);

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString sql=in.readLine();
        qDebug() << sql << "\n";
        if(!query.exec(sql)){
            qDebug() << query.lastError().text();
        }
    }
        // 通过分析values(E'),判断是否有二进制数据,如没有直接运行sql语句,如有则需要将16进制文本转换为blob数据
//        QRegExp reg("E'([0-9a-f]{1,})'");
//        if(!sql.contains(reg))
//        {
//            query.exec(sql);
//        }else
//        {
//            int pos=0;
//            QStringList bList;

//            // 探索所有的blob字段
//            while((pos=reg.indexIn(sql,pos))!=-1)
//            {
//                bList.append(reg.cap(0));

//                QString blob=reg.cap(1);
//                pos+=reg.matchedLength();
//            }

//            // blob字段填充占位符
//            foreach(QString key,bList)
//            {
//                sql.replace(key,"?");
//            }

//            query.prepare(sql);

//            // 绑定占位符数据
//            for(int i=0;i<bList.size();i++)
//            {
//                // 去除E''
//                QString hexBlob=bList[i].mid(2,bList[i].size()-1);
//                // 还原16进制数据
//                QByteArray ba=QByteArray::fromHex(hexBlob.toLocal8Bit());

//                query.bindValue(i,ba);
//            }

//            query.exec();
//        }
}

void RobotMainWindow::clearDB()
{
    QSqlQuery query;
    query.exec("DELETE FROM sensorvariable;"
               "DELETE FROM property;");
}

void RobotMainWindow::fileOpen()
{
    if (!okToClearData())
        return;
    const QString &filename = QFileDialog::getOpenFileName(this,
            tr("%1 - Open").arg(QApplication::applicationName()),
            ".", tr("robot Program (*.rbp)")/*tr("Page Designer (*.pd)")*/);
    if (filename.isEmpty())
        return;
    setWindowFilePath(filename);
    loadFile();
    QString filepath = filename.left(filename.lastIndexOf('.'));
    dbFileName = QString("%1.sql").arg(filepath);
    loadDB(dbFileName);
    if(QFile::exists(QString("%1.txt").arg(filepath)))
        proFileName = QString("%1.txt").arg(filepath);
}

void RobotMainWindow::fileNew()
{
    if (!okToClearData())
        return;
    clear();
    setWindowFilePath(tr("Unnamed"));
    setDirty(false);
}

bool RobotMainWindow::fileSave()
{
    MyGraphicsScene *scene = static_cast<MyGraphicsScene*>(((GraphicWidget *)centralWidget())->getScene());
    const QString filename = windowFilePath();
    if (filename.isEmpty() || filename == tr("Unnamed"))
        return fileSaveAs();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&file);
    out << MagicNumber << VersionNumber;
    out.setVersion(QDataStream::Qt_5_7);
    writeItems(out, scene->items());
    file.close();
    QString filepath = windowFilePath();
    dbFileName = QString("%1.sql").arg(filepath.left(filepath.lastIndexOf(('.'))));
    saveDB(dbFileName);
    setDirty(false);
    return true;
}

bool RobotMainWindow::fileSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,
            tr("%1 - Save As").arg(QApplication::applicationName()),
            ".", tr("robot Program (*.rbp)"));
    if (filename.isEmpty())
        return false;
    if (!filename.toLower().endsWith(".rbp"))
        filename += ".rbp";
    setWindowFilePath(filename);
    return fileSave();
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
    QString filepath = windowFilePath();
    proFileName = QString("%1.txt").arg(filepath.left(filepath.lastIndexOf('.')));
//    QFile file("e:/Robotprogram/demo.txt");
    QFile file(proFileName);
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
//        if(query.value(1).toString() == NULL)
//            in << "/\n";
//        else
//            in << query.value(1).toString();
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
        if(query.value(0).toString() == "JS"){
            in << "J_S_.\n";
        }else
            in << query.value(1).toString() << ":\n";
        /************单独处理判断模块**************/
        if(query.value(0).toString() == "PD"){
            in << "IF " << query.value(5).toString() << "\n";
            in << "GOTO:" << query.value(3).toString() << ";\n";
            if(query.value(4).toString() == NULL)
//                in << "ELSE GOTO:/\n";
                in << "ELSE GOTO:;\n";
            else
                in << "ELSE GOTO:" << query.value(4).toString() << ";\n";
        }
        else{
            QString contentstr = query.value(5).toString().replace(';', ";\n");
            if(contentstr == NULL)
//                in << "/\n";
                in << "";
            else if(contentstr.endsWith('\n'))
                in << contentstr;
            else
                in << contentstr << '\n';
            if(query.value(0).toString() != "JS")
                in << "GOTO:" << query.value(3).toString() << ";\n";
        }
    }

    file.close();
}

bool RobotMainWindow::download()
{
    QString toDir = "";
    if(!QFile::exists(proFileName)){
        QMessageBox::warning(this, tr("warning"), tr("未找到文件"));
        return false;
    }
    QString filename = proFileName.right(proFileName.count() - proFileName.lastIndexOf('/') - 1);
    QFileInfoList dirinfolists = QDir::drives();
    QString avaliablepath = "";
    foreach(QFileInfo dirinfo, dirinfolists){
        QString path = dirinfo.absoluteDir().path();
        avaliablepath += path;
        if(DRIVE_REMOVABLE != GetDriveType((WCHAR *)path.utf16())) //如果设备是可移动磁盘，判断名字是否为ROBOT
        {
            continue;
        }
        WCHAR szVolumeName[256];
//        WCHAR szFileSystemName[256];
//        DWORD dwSerialNumber = 0;
//        DWORD dwMaxFileNameLength = 256;
//        DWORD dwFileSystemFlags = 0;
//        bool ret = GetVolumeInformation((WCHAR *)path.utf16(), szVolumeName, 256, &dwSerialNumber,
//                             &dwMaxFileNameLength, &dwFileSystemFlags, szFileSystemName, 256);
        bool ret = GetVolumeInformation((WCHAR *)path.utf16(), szVolumeName, 256, NULL,
                             NULL, NULL, NULL, 0);
        if(!ret){
            DWORD errorMessage = GetLastError();
            QString str = QString("磁盘搜索时发生错误, 错误代码：%1, 已搜索到磁盘：%2")
                    .arg(errorMessage)
                    .arg(avaliablepath);
            QMessageBox::critical(this, tr("error"), str);
            return false;
        }
        QString name = QString::fromUtf16((const ushort*)szVolumeName);
        name.trimmed();
//        qDebug() << name << "\n";
//        qDebug() << path;
        if(name == "ROBOT"){
//            QString serialnum = QString("%1").arg(dwSerialNumber);
            toDir = path + filename;
            qDebug() << toDir;
            break;
        }
    }
    if(toDir.isEmpty()){
        QMessageBox::critical(this, tr("error"), tr("未发现磁盘"));
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        createfile->remove(toDir);
//        if(coverFileIfExist){
//            createfile->remove(toDir);
//        }
    }

    if(!QFile::copy(proFileName, toDir))
    {
        QMessageBox::critical(this, tr("error"), tr("拷贝失败，请检查磁盘是否连接"), QMessageBox::Ok);
        return false;
    }
    return true;
}

void RobotMainWindow::about()
{
    QMessageBox::information(this, tr("关于程序"), tr("上海金步机械科技有限公司"),
                             QMessageBox::Cancel);
}
