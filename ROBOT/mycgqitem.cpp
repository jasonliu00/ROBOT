#include "mycgqitem.h"
#include <QtWidgets>

MyCGQItem::MyCGQItem(QMenu *menu, MyCGQItem::CGQType cgqtype, QGraphicsItem *parent)
    :QGraphicsPolygonItem(parent)
    ,myCGQType(cgqtype)
{
    myCGQName = CGQName[(int)myCGQType];
    myContextMenu = menu;
    myPolygon << QPointF(-50.0, -20.0) << QPointF(50.0, -20.0)
              << QPointF(50.0, 20.0) << QPointF(-50.0, 20.0);

    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QPixmap MyCGQItem::image() const
{

    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(QColor::fromRgb(QRgb(0xff6600)));
    painter.translate(125, 125);
    painter.drawPolygon(myPolygon);

    return pixmap;
}

void MyCGQItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

void MyCGQItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QFont font("宋体",12);
    painter->setFont(font);
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QColor::fromRgb(QRgb(0xff6600)));
    painter->drawPolygon(myPolygon);
    QRect rect(-40, -18, 80, 36);
    painter->drawText(rect, Qt::AlignCenter, myCGQName);
}

QDataStream &operator<<(QDataStream &out, const MyCGQItem &cgqItem)
{
    out /*<< cgqItem.cgqType() */<< cgqItem.pos()
        << cgqItem.zValue();
    return out;
}


QDataStream &operator>>(QDataStream &in, MyCGQItem &cgqItem)
{
    QPointF position;
    double z;
//    MyCGQItem::CGQType type;
    in /*>> type */>> position >> z;
    cgqItem.setPos(position);
    cgqItem.setZValue(z);
//    cgqItem.setCGQType(type);
    return in;
}
