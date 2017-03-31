#include <QtWidgets>

#include "mygraphicsscene.h"
#include "graphicwidget.h"
#include "ui_graphicwidget.h"
#include "mygraphicspolygonitem.h"
#include "mygraphicitem.h"

#include "arrow.h"
#include "itemtypes.h"
#include "startmotordialog.h"
#include "stopmotordialog.h"
#include <QSqlQuery>
#include <QSqlError>


GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicWidget)
{
//    ui->setupUi(this);
    QSqlQuery query;
    //记录属性信息
    bool result = query.exec("CREATE TABLE IF NOT EXISTS property"
               "("
                   "no	INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                   "type	VARCHAR(20) NOT NULL,"
                   "name	VARCHAR(20) NOT NULL,"
                   "innum      INT UNSIGNED NOT NULL DEFAULT '0',"   //记录模块输入点的个数
                   "out0    VARCHAR(20) DEFAULT NULL,"
                   "out1    VARCHAR(20) DEFAULT NULL,"
                   "content VARCHAR(40) DEFAULT NULL,"
                   "UNIQUE INDEX (no),"
                   "PRIMARY KEY (name)"
               ")ENGINE = InnoDB;");

    qDebug() << "the Create property table query result is " << result;
    if(!result)
        qDebug() << query.lastError().text();
    //记录传感器模块
    result = query.exec("CREATE TABLE IF NOT EXISTS sensorvariable"
                        "("
                            "no INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                            "name    VARCHAR(20) NOT NULL,"
                            "UNIQUE INDEX (no),"
                            "PRIMARY KEY (name)"
                        ")ENGINE = InnoDB;");
    qDebug() << "the Create sensorvariable table query result is " << result;
    //记录添加到界面中的模块数量，只增不减
    result = query.exec("CREATE TABLE IF NOT EXISTS itemcount"
                        "("
                            "no    INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                            "type  VARCHAR(20) NOT NULL,"
                            "num   INT UNSIGNED NOT NULL DEFAULT 0,"
                            "UNIQUE INDEX(no),"
                            "PRIMARY KEY(type)"
                        ")ENGINE = InnoDB;");
    if(!result)
        qDebug() << query.lastError().text();
    qDebug() << "the Create itemcount table query result is " << result;
    createAction();
    itemMenu = new QMenu(tr("Item"));
    itemMenu->addAction(deleteAction);
    scene = new MyGraphicsScene(itemMenu, this);
    scene->setSceneRect(0, 0, 1000, 1000);
    createToolBox();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    view = new QGraphicsView(scene);
    mainLayout->addWidget(toolBox);
    mainLayout->addWidget(view);

    setLayout(mainLayout);

    connect(scene, SIGNAL(cgqItemInserted(MyCGQItem*)),
            this, SLOT(cgqItemInserted(MyCGQItem*)));
    connect(scene, SIGNAL(zxqItemInserted(MyZXQItem*)),
            this, SLOT(zxqItemInserted(MyZXQItem*)));
    connect(scene, SIGNAL(kzqItemInserted(MyKZQItem*)),
            this, SLOT(kzqItemInserted(MyKZQItem*)));
}

GraphicWidget::~GraphicWidget()
{
    delete ui;
    QSqlQuery query;
    if(query.exec("DROP TABLES property, sensorvariable, itemcount;")){
        qDebug() << "TABLE property、sensorvariable and itemcount have been successfully dropped!";
    }
}

void GraphicWidget::createToolBox()
{
    cgqButtonGroup = new QButtonGroup(this);
    cgqButtonGroup->setExclusive(false);

    connect(cgqButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(cgqButtonGroupClicked(int)));

    QGridLayout *cgqLayout = new QGridLayout;
    cgqLayout->addWidget(createCGQCellWidget(tr("光照传感器"), MyCGQItem::GZ_S,
                                                  ":/images/GZ_S.PNG"), 0, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("温度传感器"), MyCGQItem::WD_S,
                                                  ":/images/WSD_S.PNG"), 0, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("避障传感器"), MyCGQItem::BZ_S,
                                                  ":/images/BZ_S.PNG"), 1, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("声音传感器"), MyCGQItem::SY_S,
                                                  ":/images/SY_S.PNG"), 1, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("按钮检测"), MyCGQItem::AN_S,
                                                  ":/images/AN_S.PNG"), 2, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("湿度检测"), MyCGQItem::DH_S,
                                                  ":/images/COUNTER.PNG"), 2, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("火焰传感器"), MyCGQItem::HY_S,
                                                  ":/images/HY_S.PNG"), 3, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("超声波传感器"), MyCGQItem::CS_S,
                                                  ":/images/CSB_S.PNG"), 3, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("循迹传感器"), MyCGQItem::XJ_S,
                                                  ":/images/XJ_S.PNG"), 4, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("空气质量传感器"), MyCGQItem::KQ_S,
                                                  ":/images/KQ_S.PNG"), 4, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("称重传感器"), MyCGQItem::ZL_S,
                                                  ":/images/ZL_S.PNG"), 5, 0);

    cgqLayout->setRowStretch(6, 10);
    cgqLayout->setColumnStretch(2, 10);

    QWidget *cgqWidget = new QWidget;
    cgqWidget->setLayout(cgqLayout);

    zxqButtonGroup = new QButtonGroup;
    zxqButtonGroup->setExclusive(false);
    connect(zxqButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(zxqButtonGroupClicked(int)));

    QGridLayout *zxqLayout = new QGridLayout;
    zxqLayout->addWidget(createZXQCellWidget(tr("马达启动"), MyZXQItem::MotorStart,
                                                           ":/images/Mstart.PNG"), 0, 0);
    zxqLayout->addWidget(createZXQCellWidget(tr("马达停止"), MyZXQItem::MotorStop,
                                                           ":/images/Mstop.PNG"), 0, 1);
    zxqLayout->addWidget(createZXQCellWidget(tr("显示"), MyZXQItem::Show,
                                                           ":/images/show.PNG"), 1, 0);
    zxqLayout->addWidget(createZXQCellWidget(tr("指示灯"), MyZXQItem::Light,
                                                           ":/images/light.PNG"), 1, 1);
    zxqLayout->addWidget(createZXQCellWidget(tr("蜂鸣器"), MyZXQItem::Ring,
                                                           ":/images/ring.PNG"), 2, 0);
    zxqLayout->addWidget(createZXQCellWidget(tr("延时"), MyZXQItem::Delay,
                                                           ":/images/delay.PNG"), 2, 1);

    zxqLayout->setRowStretch(3, 10);
    zxqLayout->setColumnStretch(2, 10);

    QWidget *zxqWidget = new QWidget;
    zxqWidget->setLayout(zxqLayout);

    kzqButtonGroup = new QButtonGroup;
    kzqButtonGroup->setExclusive(false);  //true状态下只能有一个Button被选中，并且取消当前选中项必须通过选中其他项，否则不能取消当前的选中状态
    connect(kzqButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(kzqButtonGroupClicked(int)));

    QGridLayout *kzqLayout = new QGridLayout;
    kzqLayout->addWidget(createKZQCellWidget(tr("开始"), MyKZQItem::Begain,
                                             ":/images/begain.PNG"), 0, 0);
    kzqLayout->addWidget(createKZQCellWidget(tr("结束"), MyKZQItem::End,
                                             ":/images/end.PNG"), 0, 1);
    kzqLayout->addWidget(createKZQCellWidget(tr("条件判断"), MyKZQItem::Panduan,
                                             ":/images/panduan.PNG"), 1, 0);
    kzqLayout->setRowStretch(2, 10);
    kzqLayout->setColumnStretch(2, 10);

    QWidget *kzqWidget = new QWidget;
    kzqWidget->setLayout(kzqLayout);

    toolBox = new QToolBox;
    toolBox->setMinimumWidth(cgqWidget->sizeHint().width());
    toolBox->addItem(cgqWidget, tr("传感器模块库"));
    toolBox->addItem(zxqWidget, tr("执行器模块库"));
    toolBox->addItem(kzqWidget, tr("控制器模块库"));
}

void GraphicWidget::createAction()
{
    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteItem()));
}

void GraphicWidget::cgqButtonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = cgqButtonGroup->buttons();
    foreach(QAbstractButton *button, buttons){
        if(cgqButtonGroup->button(id) != button)
            button->setChecked(false);
    }
    if(zxqButtonGroup->checkedButton())
        zxqButtonGroup->checkedButton()->setChecked(false);
    if(kzqButtonGroup->checkedButton())
        kzqButtonGroup->checkedButton()->setChecked(false);
    scene->setMode(MyGraphicsScene::InsertCGQItem);
    scene->setCGQType(MyCGQItem::CGQType(id));
}

void GraphicWidget::zxqButtonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = zxqButtonGroup->buttons();
    foreach(QAbstractButton *button, buttons){
        if(zxqButtonGroup->button(id) != button)
            button->setChecked(false);
    }
    if(cgqButtonGroup->checkedButton())
        cgqButtonGroup->checkedButton()->setChecked(false);
    if(kzqButtonGroup->checkedButton())
        kzqButtonGroup->checkedButton()->setChecked(false);
    scene->setMode(MyGraphicsScene::InsertZXQItem);
    scene->setZXQType(MyZXQItem::ZXQType(id));
}

void GraphicWidget::kzqButtonGroupClicked(int id)
{
    QList<QAbstractButton*>buttons = kzqButtonGroup->buttons();
    foreach(QAbstractButton *button, buttons){
        if(kzqButtonGroup->button(id) != button)
            button->setChecked(false);
    }
    if(zxqButtonGroup->checkedButton())
        zxqButtonGroup->checkedButton()->setChecked(false);
    if(cgqButtonGroup->checkedButton())
        cgqButtonGroup->checkedButton()->setChecked(false);
    scene->setMode(MyGraphicsScene::InsertKZQItem);
    scene->setKZQType(MyKZQItem::KZQType(id));
}

void GraphicWidget::cgqItemInserted(MyCGQItem *item)
{
    cgqButtonGroup->button(static_cast<int>(item->cgqType()))->setChecked(false);
    scene->setMode(MyGraphicsScene::MoveItem);
}

void GraphicWidget::zxqItemInserted(MyZXQItem *item)
{
    zxqButtonGroup->button(static_cast<int>(item->zxqType()))->setChecked(false);
    scene->setMode(MyGraphicsScene::MoveItem);

}

void GraphicWidget::kzqItemInserted(MyKZQItem *item)
{
    kzqButtonGroup->button(static_cast<int>(item->kzqType()))->setChecked(false);
    scene->setMode(MyGraphicsScene::MoveItem);
}

void GraphicWidget::deleteItem()
{
    QSqlQuery query;
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if(item->type() == MyCGQItem::Type){
            scene->removeItem(item);
            QString itemname = qgraphicsitem_cast<MyCGQItem *>(item)->getCGQNameInDB();
            query.prepare("DELETE FROM sensorvariable WHERE name = :itemname;");
            query.addBindValue(itemname);
            query.exec();
            delete item;
        }else{
            if(item->type() == MyZXQItem::Type)
                 qgraphicsitem_cast<MyZXQItem *>(item)->removeArrows();
            else if(item->type() == MyKZQItem::Type){
                 MyKZQItem *tmp = qgraphicsitem_cast<MyKZQItem *>(item);
                 tmp->removeArrows();
             }
             scene->removeItem(item);
             QString modulename = ((ModelGraphicsItem*)item)->getName();
             /********记录删除模块的输出节点，并对对应的输出节点的in字段减一**********/
             query.prepare("SELECT out0, out1 FROM property WHERE name = :modulename and type <> 'JS';");
             query.addBindValue(modulename);
             if(!query.exec()){
                 qDebug() << "SELECT out0, out1 query failed in function QGraphicWidget::deleteItem()\n"
                        << query.lastError().text();
             }
             if(query.next()){
                 QString out0 = query.value(0).toString();
                 QString out1 = query.value(1).toString();
                 query.prepare("SELECT innum FROM property WHERE name = :out0;");
                 query.addBindValue(out0);
                 if(!query.exec()){
                     qDebug() << "SELECT innum of out0 query failed in function QGraphicWidget::deleteItem()\n"
                            << query.lastError().text();
                 }
                 if(query.next()){
                     int out0_in_num = query.value(0).toInt() - 1;
                     query.prepare("UPDATE property SET innum = :num WHERE name = :out0;");
                     query.addBindValue(out0_in_num);
                     query.addBindValue(out0);
                     query.exec();
                 }
                 query.prepare("SELECT innum FROM property WHERE name = :out1;");
                 query.addBindValue(out1);
                 if(!query.exec()){
                     qDebug() << "SELECT innum of out1 query failed in function QGraphicWidget::deleteItem()\n"
                            << query.lastError().text();
                 }
                 if(query.next()){
                     int out1_in_num = query.value(0).toInt() - 1;
                     query.prepare("UPDATE property SET innum = :num WHERE name = :out1;");
                     query.addBindValue(out1_in_num);
                     query.addBindValue(out1);
                     query.exec();
                 }

             }
             /***********删除模块的记录，并更新受影响的记录的out字段***********/
             query.prepare("DELETE FROM property WHERE name = :modulename;");
             query.addBindValue(modulename);
             if(!query.exec()){
                 qDebug() << "DELETE module failed\n"
                          << query.lastError().text();
             }
             query.prepare("UPDATE property SET out0 = NULL WHERE out0 = :modulename;");
             query.addBindValue(modulename);
             if(!query.exec()){
                 qDebug() << "UPDATE property out0 where modules connect to the delete items failed\n"
                          << query.lastError().text();
             }
             query.prepare("UPDATE property SET out1 = NULL WHERE out1 = :modulename;");
             query.addBindValue(modulename);
             if(!query.exec()){
                 qDebug() << "UPDATE property out1 where modules connect to the delete items failed\n"
                          << query.lastError().text();
             }
             delete item;
        }
    }
}

QWidget *GraphicWidget::createCGQCellWidget(const QString &text, MyCGQItem::CGQType cgqType, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setCheckable(true);
//    button->setFixedSize(50, 50);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    cgqButtonGroup->addButton(button, (int)cgqType);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
    QWidget *widget =  new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget *GraphicWidget::createZXQCellWidget(const QString &text, MyZXQItem::ZXQType zxqType, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setCheckable(true);

    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    zxqButtonGroup->addButton(button, (int)zxqType);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget =  new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget *GraphicWidget::createKZQCellWidget(const QString &text, MyKZQItem::KZQType kzqType, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setCheckable(true);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    kzqButtonGroup->addButton(button, (int)kzqType);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

