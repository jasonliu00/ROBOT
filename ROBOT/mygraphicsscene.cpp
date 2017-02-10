
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
        case InsertCGQItem:
            cgqItem = new MyCGQItem(myItemMenu, cgqtype);
            addItem(cgqItem);
            cgqItem->setPos(mouseEvent->scenePos());
            emit cgqItemInserted(cgqItem);
        break;
        case InsertZXQItem:{
            zxqItem = new MyZXQItem(myItemMenu, zxqtype);
            addItem(zxqItem);
//            connect(zxqItem, SIGNAL(readyToDrawLine()),
//                    this, SLOT(readyToDrawLine()));
            zxqItem->setPos(mouseEvent->scenePos());
            /****成功连线之后，更新属性表的out0字段，记录模块之间的连接关系****/
            QSqlQuery query;
            if(!query.exec("SELECT COUNT(name) AS name_count FROM property WHERE type = 'YJ';")){
                QMessageBox::warning(NULL, "Query Error", "An Error has happened when compute the name_count");
                return;
            }
            query.next();
            int name_count = query.value("name_count").toInt();
            QString myname = "YJ_" + QString().number(name_count);
            zxqItem->setName(myname);        //将模块名字保存起来
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
            QString querystr = QString("INSERT INTO property(type, name, out0, out1, content)"
                             "VALUES('YJ', %1, NULL, NULL, NULL);").arg(myname);
            qDebug() << "the query string is " << querystr;
            /*******************************************************/
            emit zxqItemInserted(zxqItem);}
        break;
        case InsertKZQItem:{
            if(kzqtype == MyKZQItem::Begain && this->isBegainModelExist()){
                QMessageBox::warning(nullptr, tr("warning"), tr("开始和结束模块都只允许存在一个！！！"));
                break;
            }
            if(kzqtype == MyKZQItem::End && this->isEndModelExist()){
                QMessageBox::warning(nullptr, tr("warning"), tr("结束模块只允许有一个"));
                break;
            }
            kzqItem = new MyKZQItem(myItemMenu, kzqtype);
            addItem(kzqItem);
            if(kzqtype == MyKZQItem::Begain)
                this->setBegainModelState(true);
            if(kzqtype == MyKZQItem::End)
                this->setEndModelState(true);
            kzqItem->setPos(mouseEvent->scenePos());
            emit kzqItemInserted(kzqItem);
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
//    QGraphicsItem *tempItem = itemAt(mouseEvent->pos(), QTransform());
//    if(MyZXQItemType == tempItem->type()){
//        MyZXQItem *item = static_cast<MyZXQItem*>(tempItem);
//        if(item->hoverState()){
//            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
//                                        mouseEvent->scenePos()));
//            line->setPen(QPen(Qt::red, 1.5, Qt::DashLine,
//                              Qt::RoundCap, Qt::MiterJoin));
//            addItem(line);
//            myMode = InsertLine;
//        }
//    }

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

            QSqlQuery query;
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

