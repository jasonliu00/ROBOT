#ifndef MYGRAPHICSELLIPSEITEM_H
#define MYGRAPHICSELLIPSEITEM_H
#include <QGraphicsEllipseItem>

class MyGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
    explicit MyGraphicsEllipseItem(QMenu *menu, QGraphicsItem *parent = Q_NULLPTR);
    QPixmap image() const;
private:
    int startAngle;
    int spanAngle;
    QRectF myRect;
    QMenu *myContextMenu;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

};

#endif // MYGRAPHICSELLIPSEITEM_H
