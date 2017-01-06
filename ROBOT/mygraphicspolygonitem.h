#ifndef MYGRAPHICSPOLYGONITEM_H
#define MYGRAPHICSPOLYGONITEM_H
#include <QGraphicsPolygonItem>

class MyGraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
//    enum ZXQType{MotorStart, MotorStop, Show, Light, Ring, Delay};
    enum PolygonType{Star, Diamond, Rectangle};
    MyGraphicsPolygonItem(QMenu *menu, PolygonType type, /*ZXQType zxqtype,*/ QGraphicsItem *parent = 0);
    PolygonType polygonType() const{return myType;}
    QPixmap image() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QMenu *myContextMenu;
    QPolygonF myPolygon;
    PolygonType myType;
//    ZXQType myZxqType;
//    QString ZxqName;

};

#endif // MYGRAPHICSPOLYGONITEM_H
