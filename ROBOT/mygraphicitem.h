#ifndef MYGRAPHICITEM_H
#define MYGRAPHICITEM_H
#include <QGraphicsItem>

class MyGraphicsItem : public QGraphicsItem
{
public:
    MyGraphicsItem(QMenu *menu, MyGraphicsItem *parent = Q_NULLPTR);
    QPixmap image() const;
    QRectF boundingRect() const;
    enum MouseType{SizeBDiagCursor, SizeFDiagCursor, ArrowCursor};
private:
    QMenu *myContextMenu;
    QPointF curPos;
    bool isHovered;
    MouseType myMouseType;
//    QRectF myPaintRect;
    QSizeF myPaintSize;
//    QRectF smallRect[4];
    bool isResizing;
    bool isInResizeArea(const QPointF &pos);
    void setPaintRect();
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

};

#endif // MYGRAPHICITEM_H
