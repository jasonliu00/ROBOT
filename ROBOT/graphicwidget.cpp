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


GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicWidget)
{
//    ui->setupUi(this);
    QSqlQuery query;
    bool result = query.exec("CREATE TABLE IF NOT EXISTS property"
               "("
                   "no	INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                   "type	VARCHAR(20) NOT NULL,"
                   "name	VARCHAR(20) NOT NULL,"
                   "out0    VARCHAR(20),"
                   "out1    VARCHAR(20),"
                   "content VARCHAR(40),"
                   "UNIQUE INDEX (no),"
                   "PRIMARY KEY (name)"
               ")ENGINE = InnoDB;");

    qDebug() << "the query result is " << result;
    createAction();
    itemMenu = new QMenu(tr("Item"));
    itemMenu->addAction(deleteAction);
//    itemMenu->addAction(propertyAction);
    scene = new MyGraphicsScene(itemMenu, this);
    scene->setSceneRect(0, 0, 1000, 1000);
    createToolBox();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    view = new QGraphicsView(scene);
    mainLayout->addWidget(toolBox);
    mainLayout->addWidget(view);

    setLayout(mainLayout);

//    connect(scene, SIGNAL(ellipseItemInserted(MyGraphicsEllipseItem*)),
//            this, SLOT(ellipseItemInserted(MyGraphicsEllipseItem*)));
//    connect(scene, SIGNAL(polygonItemInserted(MyGraphicsPolygonItem*)),
//            this, SLOT(polygonItemInserted(MyGraphicsPolygonItem*)));
//    connect(scene, SIGNAL(myItemInserted(MyGraphicsItem *)),
//            this, SLOT(myItemInserted(MyGraphicsItem*)));
    connect(scene, SIGNAL(cgqItemInserted(MyCGQItem*)),
            this, SLOT(cgqItemInserted(MyCGQItem*)));
    connect(scene, SIGNAL(zxqItemInserted(MyZXQItem*)),
            this, SLOT(zxqItemInserted(MyZXQItem*)));
}

GraphicWidget::~GraphicWidget()
{
    delete ui;
    QSqlQuery query;
    if(query.exec("DROP TABLE property")){
        qDebug() << "TABLE property has been successfully dropped!";
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
    cgqLayout->addWidget(createCGQCellWidget(tr("温湿度传感器"), MyCGQItem::WSD_S,
                                                  ":/images/WSD_S.PNG"), 0, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("避障传感器"), MyCGQItem::BZ_S,
                                                  ":/images/BZ_S.PNG"), 1, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("声音传感器"), MyCGQItem::SY_S,
                                                  ":/images/SY_S.PNG"), 1, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("按钮检测"), MyCGQItem::AN_S,
                                                  ":/images/AN_S.PNG"), 2, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("计数器"), MyCGQItem::COUNTER_S,
                                                  ":/images/COUNTER.PNG"), 2, 1);
    cgqLayout->addWidget(createCGQCellWidget(tr("火焰传感器"), MyCGQItem::HY_S,
                                                  ":/images/HY_S.PNG"), 3, 0);
    cgqLayout->addWidget(createCGQCellWidget(tr("超声波传感器"), MyCGQItem::CSB_S,
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
//    backgroundButtonGroup = new QButtonGroup;
//    backgroundButtonGroup->setExclusive(true);
//    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
//            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));


    toolBox = new QToolBox;
    toolBox->setMinimumWidth(cgqWidget->sizeHint().width());
    toolBox->addItem(cgqWidget, tr("传感器模块库"));
    toolBox->addItem(zxqWidget, tr("执行器模块库"));
    QWidget *kzqWidget = new QWidget;
    toolBox->addItem(kzqWidget, tr("控制器模块库"));
}

void GraphicWidget::createAction()
{
    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteItem()));
//    propertyAction = new QAction(tr("属性设置"));
//    connect(propertyAction, SIGNAL(triggered(bool)), this, SLOT(showPropertyDialog()));
}

//void GraphicWidget::buttonGroupClicked(int id)
//{
//    QList<QAbstractButton *> buttons = buttonGroup->buttons();
//    foreach(QAbstractButton *button, buttons){
//        if(buttonGroup->button(id) != button)
//            button->setChecked(false);
//    }
//    if(id == int(GraphicWidget::Ellipse))
//        scene->setMode(MyGraphicsScene::InsertEllipseItem);
//    else if(id == 10)
//        scene->setMode(MyGraphicsScene::InsertMyGraphicsItem);
//    else{
//        scene->setMode(MyGraphicsScene::InsertPolygonItem);
//        scene->setPolygonItemType(MyGraphicsPolygonItem::PolygonType(id));
//        scene->setZXQType(MyZXQItem::ZXQType(id));
//    }
//}

void GraphicWidget::cgqButtonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = cgqButtonGroup->buttons();
    foreach(QAbstractButton *button, buttons){
        if(cgqButtonGroup->button(id) != button)
            button->setChecked(false);
    }

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

    scene->setMode(MyGraphicsScene::InsertZXQItem);
    scene->setZXQType(MyZXQItem::ZXQType(id));
}

//void GraphicWidget::backgroundButtonGroupClicked(QAbstractButton *button)
//{
//    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
//    foreach (QAbstractButton *myButton, buttons) {
//        if (myButton != button)
//            button->setChecked(false);
//    }

//    QString text = button->text();
//    if(text == tr("马达启动"))
//        scene->setBackgroundBrush(QPixmap(":/images/Mstart.PNG"));
//    else if(text == tr("马达停止"))
//        scene->setBackgroundBrush(QPixmap(":/images/Mstop.PNG"));
//    else if(text == tr("显示"))
//        scene->setBackgroundBrush(QPixmap(":/images/show.PNG"));
//    else
//        scene->setBackgroundBrush(QPixmap(":/images/light.PNG"));

//    scene->update();
//    view->update();
//}

//void GraphicWidget::polygonItemInserted(MyGraphicsPolygonItem *item)
//{
//    buttonGroup->button(int(item->polygonType()) + int(GraphicWidget::Polygon))->setChecked(false);
//    scene->setMode(MyGraphicsScene::MoveItem);
//}

//void GraphicWidget::ellipseItemInserted(MyGraphicsEllipseItem *item)
//{
//    buttonGroup->button(int(MyGraphicsScene::InsertEllipseItem))->setChecked(false);
//    scene->setMode(MyGraphicsScene::MoveItem);
//}

//void GraphicWidget::myItemInserted(MyGraphicsItem *item)
//{
//    if(buttonGroup->checkedId() == 10)
//        buttonGroup->checkedButton()->setChecked(false);
//    scene->setMode(MyGraphicsScene::MoveItem);
//}

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

void GraphicWidget::deleteItem()
{
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
         if (item->type() == MyZXQItem::Type)
             qgraphicsitem_cast<MyZXQItem *>(item)->removeArrows();
         scene->removeItem(item);
         delete item;
    }
}

void GraphicWidget::showPropertyDialog()
{/*
    QGraphicsItem *item = scene->selectedItems().first();
    if(MyZXQItemType == item->type()){
        MyZXQItem *zxqitem = static_cast<MyZXQItem*> (item);
        switch(zxqitem->zxqType()){
            case MyZXQItem::MotorStart:{
                StartMotorDialog dlg;
                dlg.exec();
            break;
            }
            case MyZXQItem::MotorStop:{
                StopMotorDialog dlg;
                dlg.exec();
            break;
            }
            default:
                ;
        }
    }*/
}

//QWidget *GraphicWidget::createPolygonCellWidget(const QString &text, MyGraphicsPolygonItem::PolygonType itemType, const QString &image)
//{
////    MyGraphicsPolygonItem item(itemMenu, itemType);

//    QToolButton *button = new QToolButton;
//    button->setCheckable(true);

//    button->setIcon(QIcon(image));
//    button->setIconSize(QSize(50, 50));
//    buttonGroup->addButton(button, int(itemType) + int(Polygon));

//    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
//    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

//    QWidget *widget =  new QWidget;
//    widget->setLayout(layout);

//    return widget;
//}

QWidget *GraphicWidget::createCGQCellWidget(const QString &text, MyCGQItem::CGQType cgqType, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setCheckable(true);

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
//QWidget *GraphicWidget::createPolygonCellWidget(const QString &text, MyGraphicsPolygonItem::PolygonType itemType)
//{

//    MyGraphicsPolygonItem item(itemMenu, itemType);
//    QIcon icon(item.image());
//    QToolButton *button = new QToolButton;
//    button->setCheckable(true);
//    button->setIcon(icon);
//    button->setIconSize(QSize(50, 50));
//    buttonGroup->addButton(button, int(itemType) + int(Polygon));

//    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
//    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

//    QWidget *widget =  new QWidget;
//    widget->setLayout(layout);

//    return widget;
//}
//QWidget *GraphicWidget::createEllipseCellWidget(const QString &text, InsertItemType itemType)
//{
//    MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(itemMenu);
//    QToolButton *button = new QToolButton;
//    button->setCheckable(true);
//    button->setIcon(QIcon(QPixmap(item->image())));
//    button->setIconSize(QSize(50, 50));
//    buttonGroup->addButton(button, int(itemType));

//    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
//    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

//    QWidget *widget =  new QWidget;
//    widget->setLayout(layout);

//    return widget;
//}

//QWidget *GraphicWidget::createBackgroundCellWidget(const QString &text, const QString &image)
//{
//    QToolButton *button = new QToolButton;
//    button->setText(text);
//    button->setIcon(QIcon(image));
//    button->setIconSize(QSize(50, 50));
//    backgroundButtonGroup->addButton(button);

//    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
//    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

//    QWidget *widget = new QWidget;
//    widget->setLayout(layout);

//    return widget;
//}
