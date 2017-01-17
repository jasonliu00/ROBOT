#ifndef STOPMOTORDIALOG_H
#define STOPMOTORDIALOG_H

#include <QDialog>
#include "modelpropertydata.h"

namespace Ui {
class StopMotorDialog;
}

class StopMotorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StopMotorDialog(MStopData &data, QWidget *parent = 0);
    ~StopMotorDialog();

    MStopData data()const{return settingData;}

private:
    Ui::StopMotorDialog *ui;
    void dialogInit();
    void createConnection();

    MStopData settingData;
private slots:
    void checkBoxClicked();
    void okButtonClicked();
    void cancelButtonClicked();
};

#endif // STOPMOTORDIALOG_H
