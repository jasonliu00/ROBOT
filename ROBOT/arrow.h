#ifndef ARROW_H
#define ARROW_H

#include "myzxqitem.h"
#include "itemtypes.h"
#include <QGraphicsLineItem>
#include "modelgraphicsitem.h"


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
    enum { Type = ArrowItemType};

    Arrow(ModelGraphicsItem *startItem, ModelGraphicsItem *endItem,
          QPointF startPoint, QPointF endPoint, QGraphicsItem *parent = 0);

    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void setColor(const QColor &color) { myColor = color; }
    ModelGraphicsItem *startItem() const { return myStartItem; }
    ModelGraphicsItem *endItem() const { return myEndItem; }

    QPolygonF polyline() const{return myPolyline;}
    void setPolyline(QPolygonF polyline){myPolyline = polyline;}
    void setLineStartPoint(QPointF p){lineStartPoint = p;}
    void setLineEndPoint(QPointF p){lineEndPoint = p;}
    QPointF getStartPoint() const {return lineStartPoint;}
    QPointF getEndPoint() const {return lineEndPoint;}
    QString getStartItemName() const {return startItemNameInDB;}
    QString getEndItemName() const {return endItemNameInDB;}
    void setStartItemName(QString &name) {startItemNameInDB = name;}
    void setEndItemName(QString &name) {endItemNameInDB = name;}
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    ModelGraphicsItem *myStartItem;
    ModelGraphicsItem *myEndItem;
    QPointF lineStartPoint;
    QPointF lineEndPoint;
    bool notfirsttimedraw;
    QColor myColor;
    QPolygonF arrowHead;
    QPolygonF myPolyline;
    QString startItemNameInDB;
    QString endItemNameInDB;
    QPolygonF computePolygon() const;
//    QMenu *myContextMenu;
//    QAction *deleteAction;
};

QDataStream &operator<<(QDataStream &out, const Arrow &arrow);
QDataStream &operator>>(QDataStream &in, Arrow &arrow);

//! [0]

#endif // ARROW_H
