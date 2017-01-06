#include "arrow.h"

#include <math.h>

#include <QPen>
#include <QPainter>
#include <QtDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

const qreal Pi = 3.14;

//! [0]
Arrow::Arrow(MyZXQItem *startItem, MyZXQItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]

//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);

}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
//    QPainterPath path = QGraphicsLineItem::shape();
//    path.addPolygon(arrowHead);
//    return path;
    QPainterPath path;
    path.addPolygon(computePolygon());
    return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 8;
    painter->setPen(myPen);
    painter->setBrush(myColor);
    QPointF startPoint = myStartItem->outPosToScene();
    QPointF endPoint = myEndItem->inPosToScene();
//    qDebug() << "startPoint and endPoint are " << startPoint << endPoint;
    qreal endX = endPoint.x();
    qreal startX = startPoint.x();
    qreal endY = endPoint.y();
    qreal startY = startPoint.y();
    qreal y = endY - (endY-startY)/3;

    QPolygonF polyline;
    polyline << endPoint << QPointF(endX, y)
             << QPointF(startX, y) << startPoint;
    myPolyline.clear();
    foreach(QPointF point, polyline){
        myPolyline << point;
    }

//    QLineF line1(endPoint, QPointF(endX, y));
//    QLineF line2(QPointF(startX, y), startPoint);

//    setLine(QLineF(myEndItem->inPosToScene(), myStartItem->outPosToScene()));
//! [4] //! [5]
//! /*判断直线的相交点，然后从起点到相交点划线*/
//    QLineF centerLine(myStartItem-> pos(), myEndItem->pos());
//    qDebug() << "startItemPos = " << myStartItem-> pos();
//    qDebug() << "endItemPos = " << myEndItem-> pos();
//    QPolygonF endPolygon = myEndItem->polygon();
//    qDebug() << "endPolygon.first() = " << endPolygon.first();
//    QPointF p1 = endPolygon.first() + myEndItem->pos();   //pos返回（0，0）点的scene坐标
//    qDebug() << "p1 = " << p1;
//    QPointF p2;
//    QPointF intersectPoint;  //交点
//    QLineF polyLine;
//    qDebug() << "endPolygon.count() = " << endPolygon.count();
//    for (int i = 1; i < endPolygon.count(); ++i) {
//        p2 = endPolygon.at(i) + myEndItem->pos();
//        qDebug() << "endPolygon.at(i) = " << endPolygon.at(i);
//        qDebug() << "p2 = " << p2;
//        polyLine = QLineF(p1, p2);
////判断polyLine是否与centerLine相交，如果相交，交点提取到intersectPoint变量中
//        QLineF::IntersectType intersectType =
//            polyLine.intersect(centerLine, &intersectPoint);
//        if (intersectType == QLineF::BoundedIntersection)
//            qDebug() << "intersectPoint = " << intersectPoint;
//            break;
//        p1 = p2;
//    }
//    qDebug() << "QLineF is " << intersectPoint << myStartItem->pos();
//    setLine(QLineF(intersectPoint, myStartItem->pos()));
//! [5] //! [6]

//    double angle = ::acos(line().dx() / line().length());
//    if (line().dy() >= 0)
//        angle = (Pi * 2) - angle;

//    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
//                                    cos(angle + Pi / 3) * arrowSize);
//    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//                                    cos(angle + Pi - Pi / 3) * arrowSize);

    double angle = Pi / 2;
    QPointF arrowP1 = endPoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                         cos(angle + Pi / 3)* arrowSize);

    QPointF arrowP2 = endPoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                         cos(angle + Pi - Pi / 3)* arrowSize);
    arrowHead.clear();
    arrowHead << endPoint << arrowP1 << arrowP2;
//! [6] //! [7]
//    painter->drawLine(line1);
//    painter->drawLine(line2);
    painter->drawPolyline(polyline);
//    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
        painter->setPen(QPen(/*myColor*/Qt::red, 1, Qt::SolidLine));
//        QLineF myLine = line();
//        myLine.translate(0, 4.0);
//        painter->drawLine(myLine);
//        myLine.translate(0,-8.0);
//        painter->drawLine(myLine);
        painter->drawPolyline(polyline);
        painter->setBrush(Qt::red);
        painter->drawPolygon(arrowHead);
        }
}

void Arrow::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
//    QMenu menu;
//    QAction *deleteAction = menu.addAction("delete");
    //    menu.exec(event->screenPos());
}

QPolygonF Arrow::computePolygon() const
{
    qreal offset = 6.0;
    QPointF points[4];
    for(int i = 0; i < 4; i++){
        points[i] = myPolyline.at(i);
    }
    QPolygonF polygon;
    polygon << points[0] + QPointF(offset, 0) << points[1] + QPointF(offset, -offset)
            << points[2] + QPointF(offset, -offset) << points[3] + QPointF(offset, 0)
            << points[3] + QPointF(-offset, 0) << points[2] + QPointF(-offset, offset)
            << points[1] + QPointF(-offset, offset) << points[0] + QPointF(-offset, 0);
    return polygon;

}

//! [7]
