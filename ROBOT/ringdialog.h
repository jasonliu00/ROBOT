#ifndef RINGDIALOG_H
#define RINGDIALOG_H

#include <QDialog>
#include "modelpropertydata.h"
class QButtonGroup;

namespace Ui {
class RingDialog;
}

class RingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RingDialog(RingData ringdata, QWidget *parent = 0);
    ~RingDialog();
    RingData data() const{return ringsetting;}

private:
    Ui::RingDialog *ui;

    RingData ringsetting;
    QButtonGroup *yinfuButtonGroup;
    QButtonGroup *yinpinButtonGroup;

    void dialogInit();
    void createConnection();

private slots:
    void okButtonClicked();
    void cancelButtonClicked();
    void yinpinBGClicked(int Id);
    void yinfuBGClicked(int Id);
};

#endif // RINGDIALOG_H
