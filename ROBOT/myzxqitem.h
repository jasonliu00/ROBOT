#ifndef MYZXQITEM_H
#define MYZXQITEM_H
#include "modelgraphicsitem.h"
#include "itemtypes.h"
#include "modelpropertydata.h"
#include "modelpropertydata.h"

class Arrow;
class QAction;

class MyZXQItem : public ModelGraphicsItem
{
    Q_OBJECT
public:
    enum { Type = MyZXQItemType };
    enum ZXQType{MotorStart = 0, MotorStop, Show, Light, Ring, Delay};
    QString ZXQName[6] = {"马达启动", "马达停止", "显示", "指示灯", "蜂鸣器", "延时"};
    MyZXQItem(QMenu *menu, ZXQType zxqtype, QGraphicsItem *parent = Q_NULLPTR);
    QPixmap image() const;
    int type() const Q_DECL_OVERRIDE { return Type;}
    ZXQType zxqType() const{return myZXQType;}
    bool hoverState() const{return isHover;}
    void setZXQType(ZXQType &type) {myZXQType = type;}
    QVector<QPointF> startPointToPaintArrow(QPointF &point, bool notfirsttime);
//    QPointF startPointToPaintArrow(QPointF &point);
    QPointF endPointToPaintArrow(QPointF &point);
    int getChildType() const{return type();}
    MStartData mstartData() const{return MStart_Setting;}
    MStopData mstopData() const{return MStop_Setting;}
    RingData ringData() const{return ringsetting;}
    int delayData() const{return delaytime;}
    bool lightData() const{return lightstate;}
    QString showData() const{return showstring;}
    void setMStartData(MStartData &data) {MStart_Setting.setData(data);}
    void setMStopData(MStopData &data) {MStop_Setting.setData(data);}
    void setRingData(RingData &data) {ringsetting.setData(data);}
    void setDelayData(int &data) {delaytime = data;}
    void setLightData(bool &data) {lightstate = data;}
    void setShowData(QString &data) {showstring = data;}
//    void setData(MStartData data);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
//    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void showPropertyDlg();
signals:
//    void readyToDrawLine(void);
private:

//    QString myname;                 //用于记录该模块在属性表中的唯一标识名字
    QMenu *myContextMenu;
    QPolygonF myPolygon;
    ZXQType myZXQType;
    QString myZXQName;
    bool isHover;
/***出口入口的区域****/
    QRectF inArea;
    QRectF outArea;
/***出口入口点的坐标****/
    QPointF endPoint;
    QPointF startPoint;

    QList<Arrow *> arrows;

    QMenu *contextmenu;
    QAction *propertyAction;
    QAction *deleteAction;
/****属性对话框中用到的数据****/
    MStartData MStart_Setting;   //马达启动对话框数据
    MStopData MStop_Setting;     //马达停止对话框数据
    RingData ringsetting;        //蜂鸣器对话框数据
    int delaytime;            //延时对话框数据
    bool lightstate;             //指示灯数据
    QString showstring;          //显示对话框数据

    void drawInArc(QPainter *painter);
    void drawOutArc(QPainter *painter);
    void createContextMenu();
    void propertySettingInit();
};

QDataStream &operator<<(QDataStream &out, const MyZXQItem &zxqItem);
QDataStream &operator>>(QDataStream &in, MyZXQItem &zxqItem);

#endif // MYZXQITEM_H
