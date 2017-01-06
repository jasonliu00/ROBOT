#ifndef ARROW_H
#define ARROW_H

#include "myzxqitem.h"
#include <QGraphicsLineItem>


QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class QAction;
QT_END_NAMESPACE

//! [0]
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 17 };

    Arrow(MyZXQItem *startItem, MyZXQItem *endItem,
      QGraphicsItem *parent = 0);

    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void setColor(const QColor &color) { myColor = color; }
    MyZXQItem *startItem() const { return myStartItem; }
    MyZXQItem *endItem() const { return myEndItem; }

    QPolygonF polyline() const{return myPolyline;}
    void setPolyline(QPolygonF polyline){myPolyline = polyline;}

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    MyZXQItem *myStartItem;
    MyZXQItem *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
    QPolygonF myPolyline;

    QPolygonF computePolygon() const;
//    QMenu *myContextMenu;
//    QAction *deleteAction;
};
//! [0]

#endif // ARROW_H
