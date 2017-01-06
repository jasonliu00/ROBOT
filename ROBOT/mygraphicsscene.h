#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "mygraphicspolygonitem.h"
#include "mygraphicsellipseitem.h"
#include "mygraphicitem.h"
#include "mycgqitem.h"
#include "myzxqitem.h"

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode{InsertZXQItem, InsertCGQItem, InsertLine, InsertPolygonItem, InsertEllipseItem, InsertMyGraphicsItem, MoveItem};

    void setMode(Mode mode);
    Mode mode() const{return myMode;}
    void setPolygonItemType(MyGraphicsPolygonItem::PolygonType type);
    void setZXQType(MyZXQItem::ZXQType type){zxqtype = type;}
    void setCGQType(MyCGQItem::CGQType type){cgqtype = type;}
    void setInsertLine(){insertLine = true;}

    MyGraphicsScene(QMenu *menu, QObject *parent);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
private:
    QMenu *myItemMenu;
    Mode myMode;
    MyGraphicsPolygonItem::PolygonType myPolygonItemType;
    MyZXQItem::ZXQType zxqtype;
    MyCGQItem::CGQType cgqtype;
    MyGraphicsEllipseItem *ellipseItem;
    MyGraphicsPolygonItem *polygonItem;
    MyGraphicsItem *myItem;
    MyCGQItem *cgqItem;
    MyZXQItem *zxqItem;
    QGraphicsLineItem *line;

    bool insertLine;
    bool mouseDown;
signals:
    void polygonItemInserted(MyGraphicsPolygonItem *item);
    void ellipseItemInserted(MyGraphicsEllipseItem *item);
    void myItemInserted(MyGraphicsItem *item);
    void cgqItemInserted(MyCGQItem *item);
    void zxqItemInserted(MyZXQItem *item);

private slots:
//    void readyToDrawLine(void);
};

#endif // MYGRAPHICSSCENE_H
