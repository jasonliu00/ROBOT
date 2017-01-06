#include "mygraphicitem.h"
#include "mygraphicsscene.h"
#include <QtWidgets>
#include <QMenu>
const QMarginsF rectMargin = QMarginsF(2.5, 2.5, 2.5, 2.5);
MyGraphicsItem::MyGraphicsItem(QMenu *menu, MyGraphicsItem *parent)
    :QGraphicsItem(parent)
    ,isHovered(false)
    ,myPaintSize(200, 150)
    ,isResizing(false)
{
    myMouseType = ArrowCursor;
//    myPaintSize = QSizeF(200, 150);
//    myPaintRect = QRectF(QPointF(-myPaintSize.width()/2, -myPaintSize.height()/2), myPaintSize);

//    setPaintRect();
//    smallRect[0] = QRectF(myPaintRect.left() + 2.5, myPaintRect.top() + 2.5, 5, 5);
//    smallRect[1] = QRectF(myPaintRect.right() - 2.5, myPaintRect.top() + 2.5, 5, 5);
//    smallRect[2] = QRectF(myPaintRect.right() - 2.5, myPaintRect.bottom() - 2.5, 5, 5);
//    smallRect[3] = QRectF(myPaintRect.left() + 2.5, myPaintRect.bottom() - 2.5, 5, 5);

    myContextMenu = menu;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setOffset(5, 5);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(8);
    setGraphicsEffect(shadowEffect);

    setAcceptHoverEvents(true);

}

QPixmap MyGraphicsItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::magenta);
    painter.translate(125, 125);
    painter.drawRect(QRectF(-100, -75, 200, 150));

    return pixmap;
}

QRectF MyGraphicsItem::boundingRect() const
{
//    qreal penWidth = 0;
//    return QRectF(-100 - penWidth/2, -75 - penWidth/2,
//                  200 + penWidth/2, 150 + penWidth/2);
    return QRectF(QPointF(-myPaintSize.width()/2, -myPaintSize.height()/2), myPaintSize);
}


bool MyGraphicsItem::isInResizeArea(const QPointF &pos)
{
//    if(smallRect[0].contains(pos) || smallRect[1].contains(pos)
//            ||smallRect[2].contains(pos) || smallRect[3].contains(pos))
//        return true;
//    else
//        return false;
    qreal dx = myPaintSize.width()/2;
    qreal dy = myPaintSize.height()/2;
    QRectF rect1(-dx + 1, -dy + 1, 10, 10);
    QRectF rect2(dx - 11, -dy + 1, 10, 10);
    QRectF rect3(dx - 11, dy - 11, 10, 10);
    QRectF rect4(-dx + 1, dy - 11, 10, 10);
    if(rect1.contains(pos) || rect3.contains(pos)){
        myMouseType = SizeFDiagCursor;
        return true;
    }
    if(rect2.contains(pos) || rect4.contains(pos)){
        myMouseType = SizeBDiagCursor;
        return true;
    }
    else
        return false;
}

void MyGraphicsItem::setPaintRect()
{
//    myPaintRect = QRectF(QPointF(-myPaintSize.width()/2, -myPaintSize.height()/2), myPaintSize);
//    qreal x = myPaintSize.width()/2;
//    qreal y = myPaintSize.height()/2;
//    smallRect[0] = QRectF(-x, -y, 10, 10);
//    smallRect[1] = QRectF(x - 10, -y, 10, 10);
//    smallRect[2] = QRectF(x - 10, y - 10, 10, 10);
//    smallRect[3] = QRectF(-x, y - 10, 10, 10);
//    smallRect[0].setTopLeft(QPointF(-x, -y));
//    smallRect[1].setTopLeft(QPointF(x - 10, -y));
//    smallRect[2].setTopLeft(QPointF(x - 10, y - 10));
//    smallRect[3].setTopLeft(QPointF(-x, y - 10));
//    for(int i = 0; i < 4; i++)
//        smallRect[i].setSize(QSizeF(10, 10));
}


void MyGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

void MyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
//    QRectF rect = myPaintRect.marginsRemoved(rectMargin);
//    QRectF rectTmp(-100+3, -75+3, 200-5, 150-5);
//    QRectF rect = rectTmp.marginsRemoved(rectMargin);
    qreal dx = myPaintSize.width()/2;
    qreal dy = myPaintSize.height()/2;
    qreal w = myPaintSize.width();
    qreal h = myPaintSize.height();
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

//    painter->drawRect(QRectF(-100+3, -75+3, 200-5, 150-5));
    painter->setBrush(Qt::white);
    painter->drawRect(-dx + 5 + 2, -dy + 5 + 2, w - 12, h - 12);
    if(isSelected()){
//        QColor color(QRgb(0xA4D3EE));
        QColor color(164, 211, 238, 200);
        painter->setPen(QPen(color, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->setBrush(Qt::transparent);
//        painter->drawRect(myPaintRect);
        painter->drawRect(-dx + 2.5, -dy + 2.5, w - 5, h - 5);
        if(isResizing){
            QString str = QString("(%1,%2)")
                    .arg(curPos.x(), 0, 'f', 0)
                    .arg(curPos.y(), 0, 'f', 0);

            painter->drawText(curPos.x() - 60, curPos.y() - 15, str);
        }
    }
    if(isHovered){
        painter->setPen(QPen(Qt::gray, 1));
        painter->setBrush(Qt::transparent);
        painter->drawRect(-dx + 1, -dy + 1, 10, 10);
        painter->drawRect(dx - 11, -dy + 1, 10, 10);
        painter->drawRect(dx - 11, dy - 11, 10, 10);
        painter->drawRect(-dx + 1, dy - 11, 10, 10);
    }
}

void MyGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    return QGraphicsItem::mouseDoubleClickEvent(event);
}

void MyGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    isHovered = true;
    update();
}

void MyGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    isHovered = false;
    update();
}

void MyGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(isResizing || (isInResizeArea(event->pos()) && isSelected())){
        if(myMouseType == SizeFDiagCursor)
            setCursor(Qt::SizeFDiagCursor);
        else if(myMouseType == SizeBDiagCursor)
            setCursor(Qt::SizeBDiagCursor);
    }
    else
        setCursor(Qt::ArrowCursor);

//    QGraphicsItem::hoverMoveEvent(event);
}

void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && isInResizeArea(event->pos()))
        isResizing = true;
    else
        QGraphicsItem::mousePressEvent(event);
}

void MyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && isResizing)
        isResizing = false;
    else
        QGraphicsItem::mouseReleaseEvent(event);
}

void MyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isResizing){
        curPos.setX(event->pos().x());
        curPos.setY(event->pos().y());
        qreal w = event->pos().x();
        qreal h = event->pos().y();
//        qDebug

        if(w > 0)
            myPaintSize.setWidth(w + myPaintSize.width()/2);
        if(h > 0)
            myPaintSize.setHeight(h + myPaintSize.height()/2);

        prepareGeometryChange();
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}
