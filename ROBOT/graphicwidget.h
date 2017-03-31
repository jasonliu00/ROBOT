#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QWidget>
#include "mycgqitem.h"
#include "myzxqitem.h"
#include "mykzqitem.h"

class MyGraphicsScene;
class MyZXQItem;
class MyCGQItem;
class MyKZQItem;

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
    MyGraphicsScene* getScene() const{return scene;}
    QMenu * getItemMenu() const{return itemMenu;}

private:
    void createToolBox();
    void createAction();

    QWidget *createCGQCellWidget(const QString &text, MyCGQItem::CGQType cgqType, const QString &image);
    QWidget *createZXQCellWidget(const QString &text, MyZXQItem::ZXQType zxqType, const QString &image);
    QWidget *createKZQCellWidget(const QString &text, MyKZQItem::KZQType kzqType, const QString &image);
    Ui::GraphicWidget *ui;
    QToolBox *toolBox;
    MyGraphicsScene *scene;
    QGraphicsView *view;
    QButtonGroup *cgqButtonGroup;
    QButtonGroup *zxqButtonGroup;
    QButtonGroup *kzqButtonGroup;

    InsertItemType myItemType;

    QAction *deleteAction;
    QMenu *itemMenu;
private slots:
    void cgqButtonGroupClicked(int id);
    void zxqButtonGroupClicked(int id);
    void kzqButtonGroupClicked(int id);
    void cgqItemInserted(MyCGQItem *item);
    void zxqItemInserted(MyZXQItem *item);
    void kzqItemInserted(MyKZQItem *item);
    void deleteItem();
};

#endif // GRAPHICWIDGET_H
