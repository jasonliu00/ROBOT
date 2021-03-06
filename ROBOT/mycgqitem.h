#ifndef MYCGQITEM_H
#define MYCGQITEM_H
#include <QGraphicsPolygonItem>
#include "itemtypes.h"

class MyCGQItem : public QGraphicsPolygonItem
{

public:
    enum { Type = MyCGQItemType }; //确定每种Item的类型，唯一标识
    enum CGQType{GZ_S = 0, WD_S, BZ_S, SY_S, AN_S, DH_S, HY_S, CS_S, XJ_S, KQ_S, ZL_S};
    QString CGQNameInDB[11] = {"GZ_S", "WD_S", "BZ_S", "SY_S", "AN_S", "DH_S", "HY_S",
                               "CS_S", "XJ_S", "KQ_S", "ZL_S"};
    QString CGQName[11]={"光照检测", "温度检测", "避障检测", "声音检测", "按钮检测",
                      "湿度检测", "火焰检测", "超声波检测", "循迹检测", "空气检测", "重量检测"};
    MyCGQItem(QMenu *menu, CGQType cgqtype, QGraphicsItem *parent = Q_NULLPTR);
    QPixmap image() const;

    CGQType cgqType() const{return myCGQType;}
    void setCGQType(CGQType &type) {myCGQType = type;}
    QString getCGQNameInDB() const{return CGQNameInDB[int(myCGQType)];}
    int type() const Q_DECL_OVERRIDE { return Type;}
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QMenu *myContextMenu;
    QPolygonF myPolygon;
    CGQType myCGQType;
    QString myCGQName;

};

QDataStream &operator<<(QDataStream &out, const MyCGQItem &cgqItem);
QDataStream &operator>>(QDataStream &in, MyCGQItem &cgqItem);

#endif // MYCGQITEM_H
