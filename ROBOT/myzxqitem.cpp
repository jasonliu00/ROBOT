#include "myzxqitem.h"
#include <QtWidgets>
#include "mygraphicsscene.h"
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
    :ModelGraphicsItem(parent)
    ,myZXQType(zxqtype)
    ,isHover(false)
{
    propertySettingInit();
    inArea = QRectF(-6, -27, 12, 7);
    outArea = QRectF(-6, 20, 12, 7);

    endPoint = QPointF(0, -20);
    startPoint = QPointF(0, 20);
    myZXQName = ZXQName[(int)myZXQType];
    myContextMenu = menu;
    myPolygon << QPointF(-50.0, -20.0) << QPointF(50.0, -20.0)
              << QPointF(50.0, 20.0) << QPointF(-50.0, 20.0);

    createContextMenu();

//    setPolygon(myPolygon);
//    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemIsSelectable, true);
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
//    setFlag(QGraphicsItem::ItemIsFocusable, true);

//    setAcceptHoverEvents(true); //想要接受鼠标hover的事件必须设置此句
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

QPointF MyZXQItem::endPointToPaintArrow(QPointF &point)
{
    Q_UNUSED(point);
    return mapToScene(endPoint);
}

QVector<QPointF> MyZXQItem::startPointToPaintArrow(QPointF &point, bool notfirsttime)
{
    Q_UNUSED(point);
    Q_UNUSED(notfirsttime);
    QVector<QPointF> startpoints;
    startpoints.append(startPoint);
    return mapToScene(startpoints);
}

//QPointF MyZXQItem::startPointToPaintArrow(QPointF &point)
//{
//    Q_UNUSED(point);
//    return mapToScene(startPoint);
//}

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
    QSqlQuery query;
    switch(myZXQType){
        case MotorStart:{
            StartMotorDialog dlg(MStart_Setting);
            if(dlg.exec()){
                MStart_Setting.setData(dlg.data());
                QString strtoshow = "";
                QString strtocontent = "";
                if(MStart_Setting.motorChecked[0]){
                    strtoshow += "0";
//                    strtocontent += ("RUN(0," +
//                                     QString().number(MStart_Setting.motorPower[0]) +
//                                     ");");
                    strtocontent += QString("RUN(0,%1);").arg(MStart_Setting.motorPower[0], 3, 10, QLatin1Char('0'));
                }
                if(MStart_Setting.motorChecked[1]){
                    strtoshow += "1";
//                    strtocontent += ("RUN(1," +
//                                     QString().number((MStart_Setting.motorPower[1])) +
//                                     ");\n");
                    strtocontent += QString("RUN(1,%1);").arg(MStart_Setting.motorPower[1], 3, 10, QLatin1Char('0'));
                }
                if(MStart_Setting.motorChecked[2]){
                    strtoshow += "2";
//                    strtocontent += ("RUN(2," +
//                                     QString().number((MStart_Setting.motorPower[2])) +
//                                     ");");
                    strtocontent += QString("RUN(2,%1);").arg(MStart_Setting.motorPower[2], 3, 10, QLatin1Char('0'));
                }
                if(MStart_Setting.motorChecked[3]){
                    strtoshow += "3";
//                    strtocontent += ("RUN(3," +
//                                     QString().number((MStart_Setting.motorPower[3])) +
//                                     ");");
                    strtocontent += QString("RUN(3,%1);").arg(MStart_Setting.motorPower[3], 3, 10, QLatin1Char('0'));
                }
                /**********************/
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
                update();  //更新模块表面显示
            }
        break;
        }
        case Show:{
            ShowDialog dlg(showstring);
            if(dlg.exec()){
            showstring = dlg.data();
            QString strcontent = QString("SHOW(%1);").arg(showstring);
            query.prepare("UPDATE property "            //注意空格是不可省略的
                          "SET content = :content "
                          "WHERE name = :modelname;");
            query.addBindValue(strcontent);
            query.addBindValue(this->getName());
            if(!query.exec()){
                qDebug() << "UPDATE showdialog content query failed!\n"
                         << query.lastError().text();
            }
            }
        break;
        }
        case Light:{
            LightDialog dlg(lightstate);
            if(dlg.exec()){
                lightstate = dlg.data();
                QString strcontent;
                if(lightstate)
                    strcontent = "LIGHT(1);";
                else
                    strcontent = "LIGHT(0);";
                query.prepare("UPDATE property "
                              "SET content = :content "
                              "WHERE name = :modelname;");
                query.addBindValue(strcontent);
                query.addBindValue(this->getName());
                if(!query.exec()){
                    qDebug() << "UPDATE lightdialog content query failed!\n"
                             << query.lastError().text();
                }
            }
        break;
        }
        case Ring:{
            RingDialog dlg(ringsetting);
            if(dlg.exec()){
                ringsetting = dlg.data();
                QString strcontent = QString("BEEP(%1,%2);")
                        .arg(ringsetting.yinfuTime*10000)    //乘以10000是为了将小数转化为整数，方便下位机解析
//                        .arg(ringsetting.yinfuTime, 0, 'f', 4)   //4代表精度，即小数点之后4位保留
                        .arg(yinpin[ringsetting.yinpinID], 0, 'f', 1);
                query.prepare("UPDATE property "
                              "SET content = :content "
                              "WHERE name = :modelname;");
                query.addBindValue(strcontent);
                query.addBindValue(this->getName());
                if(!query.exec()){
                    qDebug() << "UPDATE ringdialog content query failed!\n"
                             << query.lastError().text();
                }
            }
        break;
        }
        case Delay:{
            DelayDialog dlg(delaytime);
            if(dlg.exec()){
                delaytime = dlg.data();
                QString strcontent = QString("DELAY(%1);").arg(delaytime);
                query.prepare("UPDATE property SET content = :content "
                              "WHERE name = :modelname;");
                query.addBindValue(strcontent);
                query.addBindValue(this->getName());
                if(!query.exec()){
                    qDebug() << "UPDATE delaydialog content query failed!\n"
                             << query.lastError().text();
                }
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
    QAction *action = nullptr;
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
/******属性对话框中数据的初始化*******/
void MyZXQItem::propertySettingInit()
{
    for(int i = 0; i < MStart_Setting.num; i++){
        MStart_Setting.motorChecked[i] = false;
        MStart_Setting.motorPower[i] = 80;
    }
    for(int i = 0; i < 4; i++){
        MStop_Setting.motorChecked[i] = true;
    }
    delaytime = 10;
    lightstate = true;

    ringsetting.yinfuID = -1;//-1代表哪个按钮都不选中
    ringsetting.yinpinID = 8;//初始状态时的音频为到（1）
    ringsetting.yinfuTime = 1.0;//默认时间为1s

    showstring = "\\n\\n\\n\\n\\n\\n";
}

QDataStream &operator<<(QDataStream &out, const MyZXQItem &zxqItem)
{
    MyZXQItem::ZXQType zxqType = zxqItem.zxqType();
    out << zxqItem.getName() << zxqItem.pos()
        << zxqItem.zValue();

    switch(zxqType){
        case MyZXQItem::MotorStart:{
            MStartData msdata;
            msdata.setData(zxqItem.mstartData());
            for(int i = 0; i < msdata.num; i++)
                out << msdata.motorChecked[i] << msdata.motorPower[i];
            break;
        }
        case MyZXQItem::MotorStop:{
            MStopData mstopdata;
            mstopdata.setData(zxqItem.mstopData());
            for(int i = 0; i < mstopdata.num; i++)
                out << mstopdata.motorChecked[i];
            break;
        }
        case MyZXQItem::Show:{
            out << zxqItem.showData();
            break;
        }

        case MyZXQItem::Light:{
            out << zxqItem.lightData();
            break;
        }
        case MyZXQItem::Ring:{
            RingData ringdata;
            ringdata.setData(zxqItem.ringData());
            out << ringdata.yinfuID << ringdata.yinfuTime
                << ringdata.yinpinID;
            break;
        }
        case MyZXQItem::Delay:{
            out << zxqItem.delayData();
            break;
        }
        default:
        break;
    }

    return out;
}


QDataStream &operator>>(QDataStream &in, MyZXQItem &zxqItem)
{
    QPointF position;
    MyZXQItem::ZXQType zxqType = zxqItem.zxqType();
    double z;
    QString modelName;

    in >> modelName >> position >> z;

    switch(zxqType){
        case MyZXQItem::MotorStart:{
            MStartData msdata;
            for(int i = 0; i < msdata.num; i++)
                in >> msdata.motorChecked[i] >> msdata.motorPower[i];
            zxqItem.setMStartData(msdata);
            break;
        }
        case MyZXQItem::MotorStop:{
            MStopData msdata;
            for(int i = 0; i < msdata.num; i++)
                in >> msdata.motorChecked[i];
            zxqItem.setMStopData(msdata);
            break;
        }
        case MyZXQItem::Show:{
            QString str;
            in >> str;
            zxqItem.setShowData(str);
            break;
        }
        case MyZXQItem::Light:{
            bool lightstate;
            in >> lightstate;
            zxqItem.setLightData(lightstate);
            break;
        }
        case MyZXQItem::Ring:{
            RingData rdata;
            in >> rdata.yinfuID << rdata.yinfuTime
                                << rdata.yinpinID;
            zxqItem.setRingData(rdata);
            break;
        }
        case MyZXQItem::Delay:{
            int delaytime;
            in >> delaytime;
            zxqItem.setDelayData(delaytime);
            break;
        }
    }

    zxqItem.setPos(position);
    zxqItem.setZValue(z);
    zxqItem.setName(modelName);
    return in;
}
