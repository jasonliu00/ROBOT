
#include "mygraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include "mygraphicsellipseitem.h"
#include "arrow.h"
#include "itemtypes.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MyGraphicsScene::MyGraphicsScene(QMenu *menu, QObject *parent)
    :QGraphicsScene(parent)
{
    begainModelExist = false;
    endModelExist = false;
    mouseDown = false;
    insertLine = false;
    line = nullptr;
    myMode = MoveItem;
    myItemMenu = menu;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QSqlQuery query;
    if(mouseEvent->button() != Qt::LeftButton)
        return;
    mouseDown = true;
    static const QColor colorForIds[6] = {Qt::red, Qt::green, Qt::yellow,
                                          Qt::blue, Qt::magenta, Qt::cyan};
    switch(myMode){
        case InsertPolygonItem:
            polygonItem = new MyGraphicsPolygonItem(myItemMenu, myPolygonItemType/*,zxqtype*/);
            polygonItem->setBrush(colorForIds[int(myPolygonItemType)%6]);
            addItem(polygonItem);
            polygonItem->setPos(mouseEvent->scenePos());
            emit polygonItemInserted(polygonItem);
        break;
        case InsertEllipseItem:
            ellipseItem = new MyGraphicsEllipseItem(myItemMenu);
            ellipseItem->setBrush(Qt::blue);
            addItem(ellipseItem);
            ellipseItem->setPos(mouseEvent->scenePos());
            emit ellipseItemInserted(ellipseItem);
        break;
        case InsertMyGraphicsItem:
            myItem = new MyGraphicsItem(myItemMenu);
//            myItem->setBrush(Qt::magenta);
            addItem(myItem);
            myItem->setPos(mouseEvent->scenePos());
            emit myItemInserted(myItem);
        case InsertCGQItem:{
            QSqlQuery query;
            cgqItem = new MyCGQItem(myItemMenu, cgqtype);
            //enum CGQType{GZ_S = 0, WSD_S, BZ_S, SY_S, AN_S,
            //COUNTER_S, HY_S, CSB_S, XJ_S, KQ_S, ZL_S};
            switch(cgqtype){
            case MyCGQItem::GZ_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'GZ_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('GZ_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("光照传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::WSD_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'WSD_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('WSD_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("温湿度传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::BZ_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'BZ_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('BZ_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("避障传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::SY_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'SY_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('SY_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("声音传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::AN_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'AN_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('AN_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("按钮模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::COUNTER_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'COUNTER_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('COUNTER_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("计数模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::HY_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'HY_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('HY_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("火焰传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::CSB_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'CSB_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('CSB_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("超声波传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::XJ_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'XJ_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('XJ_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("循迹传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::KQ_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'KQ_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('KQ_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("空气质量传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            case MyCGQItem::ZL_S:
                query.exec("SELECT COUNT(name) FROM sensorvariable WHERE name = 'ZL_S';");
                query.next();
                if(query.value(0).toInt() == 0){
                    query.exec("INSERT INTO sensorvariable(name)"
                               "VALUES('ZL_S');");
                    addItem(cgqItem);
                    cgqItem->setPos(mouseEvent->scenePos());
                    emit cgqItemInserted(cgqItem);
                }else{
                    QMessageBox::warning(nullptr, tr("warning"), tr("称重传感器模块已经存在！"));
                    emit cgqItemInserted(cgqItem);
                    delete cgqItem;
                    break;
                }
                break;
            default:
                break;
            }
        break;
    }
        case InsertZXQItem:{
            zxqItem = new MyZXQItem(myItemMenu, zxqtype);
            addItem(zxqItem);
//            connect(zxqItem, SIGNAL(readyToDrawLine()),
//                    this, SLOT(readyToDrawLine()));
            zxqItem->setPos(mouseEvent->scenePos());

            /******每添加一个模块就将信息记录到property表中********/
            if(!query.exec("SELECT COUNT(name) AS name_count FROM property WHERE type = 'YJ';")){
                QMessageBox::warning(NULL, "Query Error", "An Error has happened when compute the name_count");
                return;
            }
            query.next();
            int name_count = query.value("name_count").toInt();
            QString myname = "YJ_" + QString().number(name_count);
            zxqItem->setName(myname);        //将模块名字保存起来
            qDebug() << "zxqItem name is " << zxqItem->getName();
            query.prepare("INSERT INTO property(type,"
                       "name,"
                       "out0,"
                       "out1,"
                       "content)"
                       "VALUES('YJ',"
                       ":name,"
                       "NULL,"
                       "NULL,"
                       "NULL);");
            query.addBindValue(myname);
            query.exec();
//            QString querystr = QString("INSERT INTO property(type, name, out0, out1, content)"
//                             "VALUES('YJ', %1, NULL, NULL, NULL);").arg(myname);
//            qDebug() << "the query string is " << querystr;
            /*******************************************************/
            emit zxqItemInserted(zxqItem);}
        break;
        case InsertKZQItem:{
            if(kzqtype == MyKZQItem::Begain){
                if(this->isBegainModelExist()){
                    QMessageBox::warning(nullptr, tr("warning"), tr("开始模块只允许存在一个！！！"));
                    break;
                }else{
                    kzqItem = new MyKZQItem(myItemMenu, kzqtype);
                    addItem(kzqItem);
                    kzqItem->setPos(mouseEvent->scenePos());
                    emit kzqItemInserted(kzqItem);
                    this->setBegainModelState(true);
                    kzqItem->setName("CSH");
                    if(!query.exec("INSERT INTO property(type, name, out0, out1, content)"
                               "VALUES('CSH', 'CSH', NULL, NULL, NULL);")){
                        qDebug()<< "Insert CSH module query failed\n"
                                << query.lastError().text();
                    }
                }
            }
            if(kzqtype == MyKZQItem::End){
                if(this->isEndModelExist()){
                    QMessageBox::warning(nullptr, tr("warning"), tr("结束模块只允许有一个"));
                    break;
                }else{
                    kzqItem = new MyKZQItem(myItemMenu, kzqtype);
                    addItem(kzqItem);
                    kzqItem->setPos(mouseEvent->scenePos());
                    emit kzqItemInserted(kzqItem);
                    this->setBegainModelState(true);
                    kzqItem->setName("JS");
                    if(!query.exec("INSERT INTO property(type, name, out0, out1, content)"
                                   "VALUES('JS', 'JS', NULL, NULL, NULL);")){
                        qDebug() << "Insert JS module query failed\n"
                                 << query.lastError().text();
                    }
                }
            }
            if(kzqtype == MyKZQItem::Panduan){
                kzqItem = new MyKZQItem(myItemMenu, kzqtype);
                addItem(kzqItem);
                kzqItem->setPos(mouseEvent->scenePos());
                emit kzqItemInserted(kzqItem);
                if(!query.exec("SELECT COUNT(name) AS name_count FROM property WHERE type = 'PD';")){
                    QMessageBox::warning(NULL, "Query Error", "An Error has happened when compute the name_count");
                    return;
                }
                query.next();
                int name_count = query.value("name_count").toInt();
                QString myname = "PD_" + QString().number(name_count);
                kzqItem->setName(myname);        //将模块名字保存起来
                query.prepare("INSERT INTO property(type,"
                           "name,"
                           "out0,"
                           "out1,"
                           "content)"
                           "VALUES('PD',"
                           ":name,"
                           "NULL,"
                           "NULL,"
                           "NULL);");
                query.addBindValue(myname);
                if(!query.exec()){
                    qDebug() << "Insert PD module failed\n"
                             << query.lastError().text();
                }
            }
        }
        break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(Qt::red, 1.5, Qt::DashLine,
                              Qt::RoundCap, Qt::MiterJoin));
            mousePressPos = mouseEvent->scenePos();
            addItem(line);
        break;
        default:
            ;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseDown && myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else /*if (myMode == MoveItem)*/ {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
//    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);
    mouseReleasePos = mouseEvent->scenePos();
    if (line != nullptr && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            (startItems.first()->type() == MyZXQItem::Type ||
             startItems.first()->type() == MyKZQItem::Type) &&
            (endItems.first()->type() == MyZXQItem::Type ||
             endItems.first()->type() == MyKZQItem::Type) &&
            startItems.first() != endItems.first()) {
            ModelGraphicsItem *startItem = dynamic_cast<ModelGraphicsItem*>(startItems.first());
            ModelGraphicsItem *endItem = dynamic_cast<ModelGraphicsItem*>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem, mousePressPos, mouseReleasePos);
            arrow->setColor(Qt::black);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
            /****成功连线之后，更新属性表的out0字段，记录模块之间的连接关系****/
            QSqlQuery query;
            query.prepare("SELECT innum FROM property WHERE name = :end_item;");
            query.addBindValue(endItem->getName());
            if(!query.exec()){
                qDebug() << "Select innum query failed\n"
                         << query.lastError().text();
                return;
            }
            query.next();
            int num = query.value(0).toInt() + 1;
            query.prepare("UPDATE property SET innum = :num WHERE name = :end_item;");
            query.addBindValue(num);
            query.addBindValue(endItem->getName());
            if(!query.exec()){
                qDebug() << "Update colum in failed\n"
                         << query.lastError().text();
                return;
            }
            /************判断模块要单独处理*************/
            if(startItem->getChildType() == MyKZQItemType &&
                    ((MyKZQItem*)startItem)->kzqType() == MyKZQItem::Panduan){
                MyKZQItem *kzqStartItem = (MyKZQItem*)startItem;
                /*********Y出口所连接的模块记录在out0字段**********/
                //if(kzqStartItem->getOutType() == MyKZQItem::Y){
                if(kzqStartItem->getPdModelOutArea().YStartArea.contains(kzqStartItem->mapFromScene(mousePressPos))){
                    query.prepare("UPDATE property "   //语句之间注意空格，否则会发生语法错误导致，query失败
                                  "SET out0 = :end_item "
                                  "WHERE name = :start_item;");
//                    qDebug() << "the endItem and startItem names are " << endItem->getName() << startItem->getName();
                    query.addBindValue(endItem->getName());
                    query.addBindValue(startItem->getName());
                    query.exec();
                }
                /*********N出口所连接的模块记录在out1字段**********/
                else{
                    query.prepare("UPDATE property "   //语句之间注意空格，否则会发生语法错误导致，query失败
                                  "SET out1 = :end_item "
                                  "WHERE name = :start_item;");
//                    qDebug() << "the endItem and startItem names are " << endItem->getName() << startItem->getName();
                    query.addBindValue(endItem->getName());
                    query.addBindValue(startItem->getName());
                    query.exec();
                }
            /*************非判断模块****************/
            }else{
                query.prepare("UPDATE property "   //语句之间注意空格，否则会发生语法错误导致，query失败
                              "SET out0 = :end_item "
                              "WHERE name = :start_item;");
                qDebug() << "the endItem and startItem names are " << endItem->getName() << startItem->getName();
                query.addBindValue(endItem->getName());
                query.addBindValue(startItem->getName());
                if(!query.exec()){
                    qDebug() << "UPDATE query failed!\n " << query.lastError().text();
                }
            }

        }
    }

    line = 0;
    mouseDown = false;
    setMode(MoveItem);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MyGraphicsScene::setMode(Mode mode)
{
    myMode = mode;
}

void MyGraphicsScene::setPolygonItemType(MyGraphicsPolygonItem::PolygonType type)
{
    myPolygonItemType = type;
}

//void MyGraphicsScene::readyToDrawLine()
//{
//    insertLine = true;
//    line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
//                                mouseEvent->scenePos()));
//    line->setPen(QPen(Qt::black, 1, Qt::DotLine,
//                      Qt::SquareCap, Qt::MiterJoin));
//    addItem(line);
//}

