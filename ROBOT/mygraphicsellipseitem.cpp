#include "mygraphicsellipseitem.h"
#include "mygraphicsscene.h"
#include <QPainter>
#include <QtWidgets>

MyGraphicsEllipseItem::MyGraphicsEllipseItem(QMenu *menu, QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent)
    ,startAngle(0)
    ,spanAngle(0)
{
    myContextMenu = menu;
    myRect = QRectF(-100.0, -100.0, 200.0, 200.0);
    setRect(myRect);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QPixmap MyGraphicsEllipseItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::blue);
    painter.translate(125, 125);            //定义坐标原点为pixmap的中心而不是左上角
    painter.drawEllipse(myRect);

    return pixmap;
}

void MyGraphicsEllipseItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
