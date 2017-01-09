#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QWidget>
#include "mycgqitem.h"
#include "myzxqitem.h"

class MyGraphicsScene;
class MyZXQItem;
class MyCGQItem;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QMenu;
QT_END_NAMESPACE

namespace Ui {
class GraphicWidget;
}

class GraphicWidget : public QWidget
{
    Q_OBJECT

public:
    enum InsertItemType{Polygon, Ellipse = 100};
    explicit GraphicWidget(QWidget *parent = 0);
    ~GraphicWidget();

private:
    void createToolBox();
    void createAction();

//    QWidget *createPolygonCellWidget(const QString &text, MyGraphicsPolygonItem::PolygonType itemType, const QString &image);
    QWidget *createCGQCellWidget(const QString &text, MyCGQItem::CGQType cgqType, const QString &image);
    QWidget *createZXQCellWidget(const QString &text, MyZXQItem::ZXQType zxqType, const QString &image);
//    QWidget *createPolygonCellWidget(const QString &text, MyGraphicsPolygonItem::PolygonType itemType);
//    QWidget *createEllipseCellWidget(const QString &text, InsertItemType itemType);
//    QWidget *createBackgroundCellWidget(const QString &text, const QString &image);
    Ui::GraphicWidget *ui;
    QToolBox *toolBox;
    MyGraphicsScene *scene;
    QGraphicsView *view;
    QButtonGroup *cgqButtonGroup;
    QButtonGroup *zxqButtonGroup;
//    QButtonGroup *backgroundButtonGroup;

    InsertItemType myItemType;

    QAction *deleteAction;
    QAction *propertyAction;
    QMenu *itemMenu;


private slots:
//    void buttonGroupClicked(int id);
    void cgqButtonGroupClicked(int id);
    void zxqButtonGroupClicked(int id);
//    void backgroundButtonGroupClicked(QAbstractButton *button);
//    void polygonItemInserted(MyGraphicsPolygonItem *item);
//    void ellipseItemInserted(MyGraphicsEllipseItem *item);
//    void myItemInserted(MyGraphicsItem *item);
    void cgqItemInserted(MyCGQItem *item);
    void zxqItemInserted(MyZXQItem *item);
    void deleteItem();
    void showPropertyDialog();
};

#endif // GRAPHICWIDGET_H
