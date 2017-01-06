#include "mygraphicspolygonitem.h"
#include <QtWidgets>
#include "math.h"

const double PI = 3.1415;
const double R = 100.0;
#define DEGREE(a) a*PI/180
MyGraphicsPolygonItem::MyGraphicsPolygonItem(QMenu *menu, PolygonType type, /*ZXQType zxqtype,*/ QGraphicsItem *parent)
    :QGraphicsPolygonItem(parent)
    ,myType(type)
//    ,myZxqType(zxqtype)
//    ,ZxqName("")
{
    myContextMenu = menu;
    double t;
    switch(myType){
        case Star:
            t = 0.38197 * R;
            myPolygon << QPointF(0, -R) << QPointF(-t*cos(DEGREE(54)), -t*sin(DEGREE(54)))
                      << QPointF(-R*cos(DEGREE(18)), -R*sin(DEGREE(18))) << QPointF(-t*cos(DEGREE(18)), t*sin(DEGREE(18)))
                      << QPointF(-R*cos(DEGREE(54)), R*sin(DEGREE(54))) << QPointF(0, t)
                      << QPointF(R*cos(DEGREE(54)), R*sin(DEGREE(54))) << QPointF(t*cos(DEGREE(18)), t*sin(DEGREE(18)))
                      << QPointF(R*cos(DEGREE(18)), -R*sin(DEGREE(18))) << QPointF(t*cos(DEGREE(54)), -t*sin(DEGREE(54)));
        break;
        case Diamond:
            myPolygon << QPointF(-50*sqrt(3.0), 0) << QPointF(0, -100.0)
                      << QPointF(50*sqrt(3.0), 0) << QPointF(0, 100.0);
        break;
        case Rectangle:
            myPolygon << QPointF(-50.0, -20.0) << QPointF(50.0, -20.0)
                      << QPointF(50.0, 20.0) << QPointF(-50.0, 20.0);
        break;
        default:
            ;
    }

//    myPolygon << QPointF(0, R) << QPointF(t*cos(DEGREE(54)), t*sin(DEGREE(54)))
//              << QPointF(R*cos(DEGREE(18)), R*sin(DEGREE(18))) << QPointF(t*cos(DEGREE(18)), -t*sin(DEGREE(18)))
//              << QPointF(R*cos(DEGREE(54)), -R*sin(DEGREE(54))) << QPointF(0, -t)
//              << QPointF(-R*cos(DEGREE(54)), -R*sin(DEGREE(54))) << QPointF(-t*cos(DEGREE(18)), -t*sin(DEGREE(18)))
//              << QPointF(-R*cos(DEGREE(18)), R*sin(DEGREE(18))) << QPointF(-t*cos(DEGREE(54)), t*sin(DEGREE(54)));
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QPixmap MyGraphicsPolygonItem::image() const
{
    static const QColor colors[6] = {Qt::red, Qt::green, Qt::yellow,
                                     Qt::blue, Qt::magenta, Qt::cyan};
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(colors[int(myType)%6]);
    painter.translate(125, 125);
    painter.drawPolygon(myPolygon);

    return pixmap;
}

void MyGraphicsPolygonItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

//void MyGraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);
//    QFont font("宋体",12);
//    painter->setFont(font);
//    painter->setPen(QPen(Qt::black, 1));
//    painter->setBrush(Qt::yellow);
//    painter->drawPolygon(myPolygon);
//    QRect rect(-40, -18, 80, 36);
//    painter->drawText(rect, Qt::AlignCenter, ZxqName);
//}
