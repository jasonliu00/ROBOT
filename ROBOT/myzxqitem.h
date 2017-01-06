#ifndef MYZXQITEM_H
#define MYZXQITEM_H
#include <QGraphicsPolygonItem>

class Arrow;

class MyZXQItem : public QGraphicsPolygonItem
{

public:
    enum { Type = UserType + 16 };
    enum ZXQType{MotorStart = 0, MotorStop, Show, Light, Ring, Delay};
    QString ZXQName[6] = {"马达启动", "马达停止", "显示", "指示灯", "蜂鸣器", "延时"};
    MyZXQItem(QMenu *menu, ZXQType zxqtype, QGraphicsItem *parent = Q_NULLPTR);
    QPixmap image() const;
    int type() const Q_DECL_OVERRIDE { return Type;}
    ZXQType zxqType() const{return myZXQType;}

    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);

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

    void drawInArc(QPainter *painter);
    void drawOutArc(QPainter *painter);
};



#endif // MYZXQITEM_H
