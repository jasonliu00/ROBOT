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
Arrow::Arrow(ModelGraphicsItem *startItem, ModelGraphicsItem *endItem,
             QPointF startPoint, QPointF endPoint, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    startItemNameInDB = startItem->getName();
    endItemNameInDB = endItem->getName();
    notfirsttimedraw = false;
    lineStartPoint = startPoint;
    lineEndPoint = endPoint;
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

//    qDebug() << myPolyline.boundingRect();
//    return myPolyline.boundingRect()
//        .normalized()
//        .adjusted(-extra, -extra, extra, extra);


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
//    qDebug() << "startItem point is " << mapFromItem(myStartItem, 0, 0)
//             << ", EndItem point is " << mapFromItem(myEndItem, 0, 0);
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
    QVector<QPointF> vector = myStartItem->startPointToPaintArrow(lineStartPoint, notfirsttimedraw);
//    QPointF startPoint = myStartItem->startPointToPaintArrow(lineStartPoint);
    QPointF startPoint = vector.at(0);
    if(vector.count() > 1)
        lineStartPoint = vector.at(1);
//    lineStartPoint = startPoint;
    QPointF endPoint = myEndItem->endPointToPaintArrow(lineEndPoint);
//    qDebug() << "startPoint and endPoint are " << startPoint << endPoint;
    QPolygonF polyline;
    qreal endX = endPoint.x();
    qreal startX = startPoint.x();
    qreal endY = endPoint.y();
    qreal startY = startPoint.y();
    qreal y = endY - (endY-startY)/3;
    if(endY < startY){
        polyline << startPoint << QPointF(startX, startY + 30)
                 << QPointF(startX - (startX - endX)/2, startY + 30)
                 << QPointF(startX - (startX - endX)/2, endY - 30)
                 << QPointF(endX, endY - 30) << endPoint;
        prepareGeometryChange();
        setLine(startX, startY + 30, endX, endY - 30);
    }else
        polyline << endPoint << QPointF(endX, y)
             << QPointF(startX, y) << startPoint;
//    qDebug() << polyline;
    myPolyline.clear();
    foreach(QPointF point, polyline){
        myPolyline << point;
    }

    double angle = Pi / 2;
    QPointF arrowP1 = endPoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                         cos(angle + Pi / 3)* arrowSize);

    QPointF arrowP2 = endPoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                         cos(angle + Pi - Pi / 3)* arrowSize);
    arrowHead.clear();
    arrowHead << endPoint << arrowP1 << arrowP2;
//! [6] //! [7]

    painter->drawPolyline(polyline);
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
        painter->setPen(QPen(/*myColor*/Qt::red, 1, Qt::SolidLine));
        painter->drawPolyline(polyline);
        painter->setBrush(Qt::red);
        painter->drawPolygon(arrowHead);
        }
    notfirsttimedraw = true;
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

QDataStream &operator<<(QDataStream &out, const Arrow &arrow)
{
    out << arrow.pos() << arrow.zValue()
        << arrow.getStartPoint() << arrow.getEndPoint();
    return out;
}


QDataStream &operator>>(QDataStream &in, Arrow &arrow)
{
    QPointF position, startpoint, endpoint;
    double z;
    in >> position >> z
       >> startpoint >> endpoint;
    arrow.setPos(position);
    arrow.setLineStartPoint(startpoint);
    arrow.setLineEndPoint(endpoint);
    arrow.setZValue(z);
    return in;
}

//! [7]
