
#include "mygraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include "mygraphicsellipseitem.h"
#include "arrow.h"
#include "itemtypes.h"
#include <QMessageBox>

MyGraphicsScene::MyGraphicsScene(QMenu *menu, QObject *parent)
    :QGraphicsScene(parent)
{
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
        case InsertZXQItem:
            zxqItem = new MyZXQItem(myItemMenu, zxqtype);
            addItem(zxqItem);
//            connect(zxqItem, SIGNAL(readyToDrawLine()),
//                    this, SLOT(readyToDrawLine()));
            zxqItem->setPos(mouseEvent->scenePos());
            emit zxqItemInserted(zxqItem);
        break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(Qt::red, 1.5, Qt::DashLine,
                              Qt::RoundCap, Qt::MiterJoin));
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
            startItems.first()->type() == MyZXQItem::Type &&
            endItems.first()->type() == MyZXQItem::Type &&
            startItems.first() != endItems.first()) {
            MyZXQItem *startItem = qgraphicsitem_cast<MyZXQItem *>(startItems.first());
            MyZXQItem *endItem = qgraphicsitem_cast<MyZXQItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(Qt::black);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
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
