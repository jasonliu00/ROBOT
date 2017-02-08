#include "mykzqitem.h"
#include "mygraphicsscene.h"
#include "arrow.h"
#include <QMenu>
#include <QAction>
#include <QPainterPath>
#include <QtWidgets>
MyKZQItem::MyKZQItem(QMenu *menu, KZQType kzqtype, QGraphicsItem *parent)
    :QObject(), QGraphicsPathItem(parent)
{
    myKZQType = kzqtype;
    myKZQName = KZQModelName[(int)kzqtype];
    myContextMenu = menu;
    createContextMenu();

    inArea = QRectF(-6, -27, 12, 7);
    outArea = QRectF(-6, 20, 12, 7);
    panduanArea.reserve(3);
    panduanArea.push_back(QRectF(-6, -37, 12, 7));
    panduanArea.push_back(QRectF(50, -6, 7, 12));
    panduanArea.push_back(QRectF(-6, 30, 12, 7));
    switch(myKZQType){
    case Begain:
        myPath.moveTo(-30, -20);
        myPath.lineTo(30, -20);
        myPath.arcTo(10, -20, 40, 40, 90, -180);
        myPath.lineTo(-30, 20);
        myPath.arcTo(-50, -20, 40, 40, 270, -180);
        break;
    case Panduan:
        myPath.moveTo(0, -30);
        myPath.lineTo(50, 0);
        myPath.lineTo(0, 30);
        myPath.lineTo(-50, 0);
        myPath.lineTo(0, -30);
        break;
    case End:
        myPath.moveTo(-30, -20);
        myPath.lineTo(30, -20);
        myPath.arcTo(10, -20, 40, 40, 90, -180);
        myPath.lineTo(-30, 20);
        myPath.arcTo(-50, -20, 40, 40, 270, -180);
        break;
    }
    setPath(myPath);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setAcceptHoverEvents(true); //想要接受鼠标hover的事件必须设置此句
}

void MyKZQItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void MyKZQItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void MyKZQItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

QPointF MyKZQItem::inPosToScene() const
{
    return mapToScene(inPoint);
}

QPointF MyKZQItem::outPosToScene() const
{
    return mapToScene(outPoint);
}

void MyKZQItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    contextmenu->exec(event->screenPos());
}

QVariant MyKZQItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}

void MyKZQItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
//    QColor startmodelcol(QColor::fromRgb(QRgb(0xffcc00)));
    QColor startmodelcol(Qt::yellow);
    QColor panduanmodelcol(Qt::red);
    myPath.setFillRule(Qt::WindingFill);
    QFont font("宋体",12);
    painter->setFont(font);
    switch(myKZQType){
    case Begain:
        painter->setBrush(startmodelcol);
        drawAngle(painter, QPointF(-3, 20), QPointF(0, 25.2), QPointF(3, 20));
        break;
    case End:
        painter->setBrush(startmodelcol);
        drawAngle(painter, QPointF(-3, -25.2), QPointF(0, -20), QPointF(3, -25.2));
        break;
    case Panduan:
        painter->setBrush(panduanmodelcol);
        drawAngle(painter, QPointF(-3, -35.2), QPointF(0, -30), QPointF(3, -35.2));
        drawAngle(painter, QPointF(50, -4), QPointF(54.2, 0), QPointF(50, 4));
        drawAngle(painter, QPointF(-4, 30), QPointF(0, 34.2), QPointF(4, 30));
        break;
    default:
        ;
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(myPath);
    painter->drawText(QRect(-30, -20, 60, 40), Qt::AlignCenter, myKZQName);
}

QRectF MyKZQItem::boundingRect() const
{
    qreal penWidth = 1;
    /****7是为上下两个三角留出的余量*****/
    if(myKZQType == Panduan)
        return QRectF(-50 - penWidth / 2, -37 - penWidth / 2,
                  107 + penWidth, 74 + penWidth);
    else
        return QRectF(-50 - penWidth / 2, -27 - penWidth / 2,
                      100 + penWidth, 54 + penWidth);
}

QPainterPath MyKZQItem::shape() const
{
    QPainterPath p;
    p.addRect(boundingRect());
    return p;
}

void MyKZQItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    switch(myKZQType){
    case Begain:
        if(outArea.contains(event->pos())){
            setCursor(Qt::CrossCursor);
            isHover = true;
            if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::InsertLine)
                ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::InsertLine);
        }
        break;
    case End:
        if(inArea.contains(event->pos())){
            setCursor(Qt::CrossCursor);
            isHover = true;
            if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::InsertLine)
                ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::InsertLine);
        }
        break;
    case Panduan:
        foreach(QRectF rect, panduanArea){
            if(rect.contains(event->pos())){
                isHover = true;
                setCursor(Qt::CrossCursor);
                if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::InsertLine)
                    ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::InsertLine);
                break;
            }
        }

        break;
    default:
        setCursor(Qt::ArrowCursor);
        isHover = false;
        if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::MoveItem)
            ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::MoveItem);
        break;
    }
}

void MyKZQItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isHover == true){
        event->ignore();
    }else
        QGraphicsItem::mousePressEvent(event);
}

void MyKZQItem::showPropertyDlg()
{

}

void MyKZQItem::drawAngle(QPainter *painter, QPointF startpoint, QPointF midpoint, QPointF endpoint)
{
    QPen pen(Qt::black, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawLine(startpoint, midpoint);
    painter->drawLine(midpoint, endpoint);
}

void MyKZQItem::createContextMenu()
{
    QAction *action;
    foreach(action, myContextMenu->actions()){
        if(action->text() == tr("&delete"))
            break;
    }
    contextmenu = new QMenu();
    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&delete"));
    connect(deleteAction, SIGNAL(triggered(bool)), action, SIGNAL(triggered(bool)));
    propertyAction = new QAction(tr("属性设置"));
    propertyAction->setCheckable(false);
    connect(propertyAction, SIGNAL(triggered(bool)), this, SLOT(showPropertyDlg()));
    contextmenu->addAction(deleteAction);
    contextmenu->addAction(propertyAction);
}
