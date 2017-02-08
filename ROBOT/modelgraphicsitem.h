#ifndef MODELGRAPHICSITEM_H
#define MODELGRAPHICSITEM_H
#include <QGraphicsObject>
class Arrow;

class ModelGraphicsItem : public QGraphicsObject
{
public:
    ModelGraphicsItem(QGraphicsItem *parent = Q_NULLPTR);
    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);
    QString getName() const{return myname;}  //用于获得模块在数据库中的唯一名字
    void setName(QString &str){myname = str;}

    void drawAngle(QPainter *painter, QPointF startpoint, QPointF midpoint, QPointF endpoint);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
private:
    QString myname;                 //用于记录该模块在属性表中的唯一标识名字

    QList<Arrow *> arrows;
};

#endif // MODELGRAPHICSITEM_H
