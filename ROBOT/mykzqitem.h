#ifndef MYKZQITEM_H
#define MYKZQITEM_H
#include <QGraphicsPathItem>
#include "itemtypes.h"
#include "modelpropertydata.h"
#include "modelgraphicsitem.h"

class QAction;
class Arrow;

struct pdModelOutArea{
    QRectF YStartArea;
    QRectF NStartArea;
};
const QPointF begainStartPoint = QPointF(0, 20);
const QPointF endEndPoint = QPointF(0, -20);
const QPointF panduanEndPoint = QPointF(0, -30);
const QPointF panduanYStartPoint = QPointF(0, 30);
const QPointF panduanNStartPoint = QPointF(50, 0);
class MyKZQItem : public ModelGraphicsItem  //两个public都需要
{
    Q_OBJECT
public:
    enum {Type = MyKZQItemType};
    enum KZQType{Begain = 0, Panduan, End};
    enum PanduanYorN{Y = 0, N};
    PanduanYorN getOutType() const{return YorNOut;}
    const QString KZQModelName[3] = {"开始", "条件判断", "结束"};
    const QPointF endPoints[3] = {QPointF(0, 0), QPointF(0, -30), QPointF(0, -20)};
    MyKZQItem(QMenu *menu, KZQType kzqtype, QGraphicsItem *parent = Q_NULLPTR);
    int type() const Q_DECL_OVERRIDE {return Type;}
    KZQType kzqType() const {return myKZQType;}
    void setKZQType(KZQType &type) {myKZQType = type;}
    bool hoverState() const{return isHover;}
    QVector<QPointF> startPointToPaintArrow(QPointF &point, bool notfirsttime);
    QPointF startPointToPaintArrow(QPointF &point);
    QPointF endPointToPaintArrow(QPointF &point);
    int getChildType() const{return type();}
    pdModelOutArea getPdModelOutArea() const{return myPdModelOutArea;}
    PdData pdDlgData() const{return panduanDialogData;}
    void setPdDlgData(PdData &data) {panduanDialogData.setData(data);}
    QList<Arrow *> arrows;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
//    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
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

    QMenu *myContextMenu;
    QPainterPath myPath;
    KZQType myKZQType;
    PanduanYorN YorNOut;
    QString myKZQName;
    bool isHover;
/***出口入口的区域****/
    QRectF inArea;
    QRectF outArea;
    pdModelOutArea myPdModelOutArea;
    QVector<QRectF> changeCursorArea;

//    QList<Arrow *> arrows;

    QMenu *contextmenu;
    QAction *propertyAction;
    QAction *deleteAction;
    PdData panduanDialogData;
    void drawAngle(QPainter *painter, QPointF startpoint, QPointF midpoint, QPointF endpoint);
    void createContextMenu();
    void propertySettingInit();
};

QDataStream &operator<<(QDataStream &out, const MyKZQItem &kzqItem);
QDataStream &operator>>(QDataStream &in, MyKZQItem &kzqItem);

#endif // MYKZQITEM_H
