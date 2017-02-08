#ifndef MYKZQITEM_H
#define MYKZQITEM_H
#include <QGraphicsPathItem>
#include "itemtypes.h"

class QAction;
class Arrow;

class MyKZQItem : public QObject, public QGraphicsPathItem   //两个public都需要
{
    Q_OBJECT
public:
    enum {Type = MyKZQItemType};
    enum KZQType{Begain = 0, Panduan, End};
    const QString KZQModelName[3] = {"开始", "条件判断", "结束"};
    MyKZQItem(QMenu *menu, KZQType kzqtype, QGraphicsItem *parent = Q_NULLPTR);
    int type() const Q_DECL_OVERRIDE {return Type;}
    KZQType kzqType() const {return myKZQType;}
    bool hoverState() const{return isHover;}

    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);
    QString getName() const{return myname;}  //用于获得模块在数据库中的唯一名字
    void setName(QString &str){myname = str;}

    QPointF inPosToScene() const;
    QPointF outPosToScene() const;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void showPropertyDlg();
signals:
//    void readyToDrawLine(void);
private:

    QString myname;                 //用于记录该模块在属性表中的唯一标识名字
    QMenu *myContextMenu;
    QPainterPath myPath;
    KZQType myKZQType;
    QString myKZQName;
    bool isHover;
/***出口入口的区域****/
    QRectF inArea;
    QRectF outArea;
    QVector<QRectF> panduanArea;
/***出口入口点的坐标****/
    QPointF inPoint;
    QPointF outPoint;

    QList<Arrow *> arrows;

    QMenu *contextmenu;
    QAction *propertyAction;
    QAction *deleteAction;
    void drawAngle(QPainter *painter, QPointF startpoint, QPointF midpoint, QPointF endpoint);
    void createContextMenu();
//    void propertySettingInit();
};

#endif // MYKZQITEM_H
