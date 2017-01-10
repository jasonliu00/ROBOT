#ifndef MYZXQITEM_H
#define MYZXQITEM_H
#include <QGraphicsPolygonItem>
#include "itemtypes.h"
#include "modelpropertydata.h"
#include "modelpropertydata.h"

class Arrow;
class QAction;

class MyZXQItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    enum { Type = MyZXQItemType };
    enum ZXQType{MotorStart = 0, MotorStop, Show, Light, Ring, Delay};
    QString ZXQName[6] = {"马达启动", "马达停止", "显示", "指示灯", "蜂鸣器", "延时"};
    MyZXQItem(QMenu *menu, ZXQType zxqtype, QGraphicsItem *parent = Q_NULLPTR);
    QPixmap image() const;
    int type() const Q_DECL_OVERRIDE { return Type;}
    ZXQType zxqType() const{return myZXQType;}
    bool hoverState() const{return isHover;}

    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);

    MSData data(){return MS_setting;}
    void setData(MSData data);

    QPointF inPosToScene() const;
    QPointF outPosToScene() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void showPropertyDlg();
signals:
//    void readyToDrawLine(void);
private:

    QMenu *myContextMenu;
    QPolygonF myPolygon;
    ZXQType myZXQType;
    QString myZXQName;
    bool isHover;

    QRectF inArea;
    QRectF outArea;

    QPointF inPoint;
    QPointF outPoint;

    QList<Arrow *> arrows;

    QMenu *contextmenu;
    QAction *propertyAction;
    QAction *deleteAction;

    MSData MS_setting;
    void drawInArc(QPainter *painter);
    void drawOutArc(QPainter *painter);
    void createContextMenu();
};



#endif // MYZXQITEM_H
