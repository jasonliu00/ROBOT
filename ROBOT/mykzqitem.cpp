#include "mykzqitem.h"
#include "mygraphicsscene.h"
#include "arrow.h"
#include "panduandialog.h"
#include <QMenu>
#include <QAction>
#include <QPainterPath>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlError>
MyKZQItem::MyKZQItem(QMenu *menu, KZQType kzqtype, QGraphicsItem *parent)
    :ModelGraphicsItem(parent)
{
    /*****菱形的外接矩形为100*60*****/
    myPdModelOutArea.NStartArea = QRectF(40, -5, 17, 10);
    myPdModelOutArea.YStartArea = QRectF(-10, 20, 20, 14);
    myKZQType = kzqtype;
    myKZQName = KZQModelName[(int)kzqtype];
    myContextMenu = menu;
    createContextMenu();
    propertySettingInit();
    inArea = QRectF(-6, -27, 12, 7);
    outArea = QRectF(-6, 20, 12, 7);
    changeCursorArea.reserve(3);
    changeCursorArea.push_back(QRectF(-6, -37, 12, 7));
    changeCursorArea.push_back(QRectF(50, -6, 7, 12));
    changeCursorArea.push_back(QRectF(-6, 30, 12, 7));
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
//    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemIsSelectable, true);
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
//    setFlag(QGraphicsItem::ItemIsFocusable, true);

//    setAcceptHoverEvents(true); //想要接受鼠标hover的事件必须设置此句
}

QVector<QPointF> MyKZQItem::startPointToPaintArrow(QPointF &point, bool notfirsttime)
{
    QVector<QPointF> vector;
    vector.reserve(2);
    if(myKZQType == Begain){
        vector.append(QPointF(0, 20));
        return mapToScene(vector);
    }
    if(myKZQType == Panduan){
        if(notfirsttime == true){
            vector.append(mapToScene(point));
            vector.append(point);
            return vector;
        }else{
            QPointF tempp = mapFromScene(point);
            if(myPdModelOutArea.NStartArea.contains(tempp))
            {
                vector.append(mapToScene(QPointF(50, 0)));
                vector.append(QPointF(50, 0));
                YorNOut = N;
                return vector;
            }
            if(myPdModelOutArea.YStartArea.contains(tempp)){
                vector.append(mapToScene(QPointF(0, 30)));
                vector.append(QPointF(0, 30));
                YorNOut = Y;
                return vector;
            }
        }
    }
//    qDebug() << "Error happend. point is not within avaliable area.";
    vector.append(QPointF(0, 0));
    return vector;
}

QPointF MyKZQItem::startPointToPaintArrow(QPointF &point)
{
    switch(myKZQType){
        case Begain:
        return mapToScene(begainStartPoint);
        break;
        case Panduan:
        if(myPdModelOutArea.YStartArea.contains(point))
            return mapToScene(panduanYStartPoint);
        if(myPdModelOutArea.NStartArea.contains(point))
            return mapToScene(panduanNStartPoint);
        break;
        default:
        break;
    }
}

QPointF MyKZQItem::endPointToPaintArrow(QPointF &point)
{
    Q_UNUSED(point);
    return mapToScene(endPoints[int(myKZQType)]);
}

void MyKZQItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    contextmenu->exec(event->screenPos());
}

//QVariant MyKZQItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//    if (change == QGraphicsItem::ItemPositionChange) {
//        foreach (Arrow *arrow, arrows) {
//            arrow->updatePosition();
//        }
//    }

//    return value;
//}

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
    if(myKZQType == Panduan){
        QFont font1("TimesNewRoman", 9);
        painter->setFont(font1);
        painter->drawText(QRect(35, -5, 10, 10), Qt::AlignCenter, "N");
        painter->drawText(QRect(-5, 15, 10, 10), Qt::AlignCenter, "Y");
    }
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
    setCursor(Qt::ArrowCursor);
    isHover = false;
    if(((MyGraphicsScene*)scene())->mode() != MyGraphicsScene::MoveItem)
        ((MyGraphicsScene*)scene())->setMode(MyGraphicsScene::MoveItem);
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
        foreach(QRectF rect, changeCursorArea){
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
        ;
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
    QSqlQuery query;
    if(myKZQType == KZQType::Panduan){
        PanduanDialog dlg(panduanDialogData);
        if(dlg.exec()){
            panduanDialogData.setData(dlg.data());
            QString content;
            if(panduanDialogData.tj2_isEnable){
                if(panduanDialogData.tj2_logicVersusTj1 == "与")
                    content = QString("%1%2%3&&%4%5%6").arg(panduanDialogData.tj1_variableName)
                                                       .arg(panduanDialogData.tj1_operator)
                                                       .arg(panduanDialogData.tj1_compareContent.toInt(),
                                                            3, 10, QLatin1Char('0')) //下位机为解析方便需要将数字补齐为3位数
                                                       .arg(panduanDialogData.tj2_variableName)
                                                       .arg(panduanDialogData.tj2_operator)
                                                       .arg(panduanDialogData.tj2_compareContent.toInt(),
                                                            3, 10, QLatin1Char('0'));
                else if(panduanDialogData.tj2_logicVersusTj1 == "或")
                    content = QString("%1%2%3||%4%5%6").arg(panduanDialogData.tj1_variableName)
                                                       .arg(panduanDialogData.tj1_operator)
                                                       .arg(panduanDialogData.tj1_compareContent.toInt(),
                                                            3, 10, QLatin1Char('0'))
                                                       .arg(panduanDialogData.tj2_variableName)
                                                       .arg(panduanDialogData.tj2_operator)
                                                       .arg(panduanDialogData.tj2_compareContent.toInt(),
                                                            3, 10, QLatin1Char('0'));
                else
                    content = QString("%1%2%3!%4%5%6").arg(panduanDialogData.tj1_variableName)
                                                       .arg(panduanDialogData.tj1_operator)
                                                       .arg(panduanDialogData.tj1_compareContent.toInt(),
                                                            3, 10, QLatin1Char('0'))
                                                       .arg(panduanDialogData.tj2_variableName)
                                                       .arg(panduanDialogData.tj2_operator)
                                                       .arg(panduanDialogData.tj2_compareContent.toInt(),
                                                            3, 10, QLatin1Char('0'));
            }else{
                content = QString("%1%2%3").arg(panduanDialogData.tj1_variableName)
                                                   .arg(panduanDialogData.tj1_operator)
                                                   .arg(panduanDialogData.tj1_compareContent.toInt(),
                                                        3, 10, QLatin1Char('0'));
            }
            query.prepare("UPDATE property SET content = :content "
                          "WHERE name = :name;");
            query.addBindValue(content);
            query.addBindValue(this->getName());
            if(!query.exec()){
                qDebug() << "UPDATE property to set panduan model content failed\n"
                         << query.lastError().text();
            }
        }
    }
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

void MyKZQItem::propertySettingInit()
{
    panduanDialogData.tj1_variableName = "";
    panduanDialogData.tj1_compareContent = "";
    panduanDialogData.tj1_operator = "==";
    panduanDialogData.tj2_isEnable = false;
    panduanDialogData.tj2_variableName = "";
    panduanDialogData.tj2_compareContent = "";
    panduanDialogData.tj2_operator = "==";
    panduanDialogData.tj2_logicVersusTj1 = "与";
}

QDataStream &operator<<(QDataStream &out, const MyKZQItem &kzqItem)
{
    MyKZQItem::KZQType kzqtype = kzqItem.kzqType();
    out << kzqItem.getName() << kzqItem.pos() << kzqItem.zValue();
    switch(kzqtype){
        case MyKZQItem::Panduan:{
            PdData data;
            data.setData(kzqItem.pdDlgData());
            out << data.tj1_compareContent << data.tj1_operator
                << data.tj1_variableName << data.tj2_compareContent
                << data.tj2_isEnable << data.tj2_logicVersusTj1
                << data.tj2_operator << data.tj2_variableName;
            break;
        }
        default:break;
    }

//    out << kzqItem.arrows.count();
//    foreach(Arrow arrow, kzqItem.arrows)
//        out << arrow;

    return out;
}


QDataStream &operator>>(QDataStream &in, MyKZQItem &kzqItem)
{
    QString name;
    QPointF position;
    double z;
    MyKZQItem::KZQType kzqtype = kzqItem.kzqType();
    in >> name >> position >> z;

    switch(kzqtype){
        case MyKZQItem::Panduan:{
            PdData data;
            in >> data.tj1_compareContent >> data.tj1_operator
               >> data.tj1_variableName >> data.tj2_compareContent
               >> data.tj2_isEnable >> data.tj2_logicVersusTj1
               >> data.tj2_operator >> data.tj2_variableName;
            kzqItem.setPdDlgData(data);
            break;
        }
        default:break;
    }
    kzqItem.setName(name);
    kzqItem.setPos(position);
    kzqItem.setZValue(z);
//    kzqItem.setKZQType(type);
    return in;
}
