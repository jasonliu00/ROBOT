#include "myzxqitem.h"
#include <QtWidgets>
#include "mygraphicsscene.h"
#include "arrow.h"
#include "startmotordialog.h"
#include "stopmotordialog.h"
#include "lightdialog.h"
#include "ringdialog.h"
#include "showdialog.h"
#include "delaydialog.h"
#include <QAction>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MyZXQItem::MyZXQItem(QMenu *menu, MyZXQItem::ZXQType zxqtype, QGraphicsItem *parent)
    :QObject(), QGraphicsPolygonItem(parent)
    ,myZXQType(zxqtype)
    ,isHover(false)
{
    propertySettingInit();
    inArea = QRectF(-6, -27, 12, 7);
    outArea = QRectF(-6, 20, 12, 7);

    inPoint = QPointF(0, -20);
    outPoint = QPointF(0, 20);
    myZXQName = ZXQName[(int)myZXQType];
    myContextMenu = menu;
    myPolygon << QPointF(-50.0, -20.0) << QPointF(50.0, -20.0)
              << QPointF(50.0, 20.0) << QPointF(-50.0, 20.0);

    createContextMenu();

    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setAcceptHoverEvents(true); //想要接受鼠标hover的事件必须设置此句
}

QPixmap MyZXQItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);
    painter.translate(125, 125);
    painter.drawPolygon(myPolygon);

    return pixmap;
}

void MyZXQItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void MyZXQItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void MyZXQItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}


QPointF MyZXQItem::inPosToScene() const
{
    return mapToScene(inPoint);
}

QPointF MyZXQItem::outPosToScene() const
{
    return mapToScene(outPoint);
}

QVariant MyZXQItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}

void MyZXQItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
//    qDebug()<< "myZXQType in contextMenuEvent() is " << myZXQType;
   // myContextMenu->exec(event->screenPos());
    contextmenu->exec(event->screenPos());
}

void MyZXQItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QFont font("宋体",12);
    painter->setFont(font);
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QColor::fromRgb(QRgb(0x00AA00)));
    painter->drawPolygon(myPolygon);
    QRect rect(-40, -18, 80, 36);
    painter->drawText(rect, Qt::AlignCenter, myZXQName);
    drawInArc(painter);
    drawOutArc(painter);
}

QRectF MyZXQItem::boundingRect() const  //决定item的重绘区域
{
    qreal penWidth = 1;
    return QRectF(-50 - penWidth / 2, -27 - penWidth / 2,
                  100 + penWidth, 54 + penWidth);
}

QPainterPath MyZXQItem::shape() const   //hittest 会调用此函数
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void MyZXQItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
//    if(inArea.contains(event->pos()) ||
//            outArea.contains(event->pos()))
//        isHover = true;
}

void MyZXQItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
//    if(inArea.contains(event->pos()) ||
//            outArea.contains(event->pos()))
//        isHover = false;
}

void MyZXQItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(inArea.contains(event->pos()) ||
       outArea.contains(event->pos())){
        setCursor(Qt::CrossCursor);
        isHover = true;
        if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::InsertLine)
            ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::InsertLine);
    }else{
        setCursor(Qt::ArrowCursor);
        isHover = false;
        if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::MoveItem)
            ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::MoveItem);
    }
}

void MyZXQItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isHover == true){
//        emit readyToDrawLine();
//        QMessageBox::information(NULL, "Item", "this is an Item Box",
//                                 QMessageBox::Ok);
        event->ignore();
    }else
        QGraphicsItem::mousePressEvent(event);
}

void MyZXQItem::showPropertyDlg()
{
//    qDebug()<< "myZXQType in showPropertyDlg() is " << myZXQType;
    switch(myZXQType){
        case MotorStart:{
            StartMotorDialog dlg(MStart_Setting);
            if(dlg.exec()){
                MStart_Setting.setData(dlg.data());
                QString strtoshow = "";
                QString strtocontent = "";
                if(MStart_Setting.motorChecked[0]){
                    strtoshow += "0";
                    strtocontent += ("RUN(0," +
                                     QString().number(MStart_Setting.motorPower[0]) +
                                     ");");
                }
                if(MStart_Setting.motorChecked[1]){
                    strtoshow += "1";
                    strtocontent += ("RUN(1," +
                                     QString().number((MStart_Setting.motorPower[1])) +
                                     ")");
                }
                if(MStart_Setting.motorChecked[2]){
                    strtoshow += "1";
                    strtocontent += ("RUN(1," +
                                     QString().number((MStart_Setting.motorPower[2])) +
                                     ")");
                }
                if(MStart_Setting.motorChecked[3]){
                    strtoshow += "1";
                    strtocontent += ("RUN(1," +
                                     QString().number((MStart_Setting.motorPower[3])) +
                                     ")");
                }
                /**********************/
                QSqlQuery query;
                query.prepare("UPDATE property "
                              "SET content = :content "
                              "WHERE name = :name;");
                query.addBindValue(strtocontent);
                query.addBindValue(this->getName());
                if(!query.exec()){
                    qDebug() << "UPDATE content query failed!\n" << query.lastError().text();
                }
                /**********************/
                if(strtoshow == "0123")
                    strtoshow = "全";
                myZXQName = "马达" + strtoshow + "启动";
                update();
            }
        break;
        }
        case MotorStop:{
            StopMotorDialog dlg(MStop_Setting);
            if(dlg.exec()){
                MStop_Setting.setData(dlg.data());
                QString strtoshow = "";
                QString strtocontent = "";
                if(MStop_Setting.motorChecked[0]){
                    strtocontent += "STOP(0);";
                    strtoshow += "0";
                }
                if(MStop_Setting.motorChecked[1]){
                    strtocontent += "STOP(1);";
                    strtoshow += "1";
                }
                if(MStop_Setting.motorChecked[2]){
                    strtocontent += "STOP(2);";
                    strtoshow += "2";
                }
                if(MStop_Setting.motorChecked[3]){
                    strtocontent += "STOP(3);";
                    strtoshow += "3";
                }
                /**********更新属性表的content字段*********/
                QSqlQuery query;
                query.prepare("UPDATE property "
                              "SET content = :content "
                              "WHERE name = :modelname;");
                query.addBindValue(strtocontent);
                query.addBindValue(this->getName());
                if(!query.exec()){
                    qDebug() << "UPDATE content query failed!\n" << query.lastError().text();
                }
                /*****************************************/
                if(strtoshow == "0123")
                    strtoshow = "全";
                myZXQName = "马达" + strtoshow + "停";
                update();
            }
        break;
        }
        case Show:{
            ShowDialog dlg;
            if(dlg.exec()){

            }
        break;
        }
        case Light:{
            LightDialog dlg(lightstate);
            if(dlg.exec()){
                lightstate = dlg.data();
            }
        break;
        }
        case Ring:{
            RingDialog dlg(ringsetting);
            if(dlg.exec()){
                ringsetting = dlg.data();
            }
        break;
        }
        case Delay:{
            DelayDialog dlg(delaytime);
            if(dlg.exec()){
                delaytime = dlg.data();
            }
        break;
        }
        default:
            ;
    }
}

void MyZXQItem::drawInArc(QPainter *painter)
{
    //QPen::QPen(const QBrush &brush, qreal width,
    //           Qt::PenStyle style = Qt::SolidLine,
    //           Qt::PenCapStyle cap = Qt::SquareCap,
    //           Qt::PenJoinStyle join = Qt::BevelJoin)
    QPen pen(Qt::black, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    QPointF point1(-3, -25.2);
    QPointF point2(0, -20);
    QPointF point3(3, -25.2);
    painter->drawLine(point1, point2);
    painter->drawLine(point2, point3);
}

void MyZXQItem::drawOutArc(QPainter *painter)
{
    QPen pen(Qt::black, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    QPointF point1(-3, 20);
    QPointF point2(0, 25.2);
    QPointF point3(3, 20);
    painter->drawLine(point1, point2);
    painter->drawLine(point2, point3);
}

void MyZXQItem::createContextMenu()
{
    QAction *action;
    foreach(action, myContextMenu->actions()){
        if(action->text() == tr("&delete"))
            break;
    }
    contextmenu = new QMenu();
    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&delete"));
    connect(deleteAction, SIGNAL(triggered(bool)), action, SIGNAL(triggered(bool)));
    propertyAction = new QAction(tr("属性设置"));
    propertyAction->setCheckable(false);
    connect(propertyAction, SIGNAL(triggered(bool)), this, SLOT(showPropertyDlg()));
    contextmenu->addAction(deleteAction);
    contextmenu->addAction(propertyAction);
}

void MyZXQItem::propertySettingInit()
{
    for(int i = 0; i < MStart_Setting.num; i++){
        MStart_Setting.motorChecked[i] = false;
        MStart_Setting.motorPower[i] = 80;
    }
    for(int i = 0; i < 4; i++){
        MStop_Setting.motorChecked[i] = true;
    }
    delaytime = 0.5;
    lightstate = true;

    ringsetting.yinfuID = -1;//-1代表哪个按钮都不选中
    ringsetting.yinpinID = 8;//初始状态时的音频为到（1）
    ringsetting.yinfuTime = 1.0;//默认时间为1s
}
