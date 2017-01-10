#ifndef STARTMOTORDIALOG_H
#define STARTMOTORDIALOG_H

#include <QDialog>
#include "modelpropertydata.h"


namespace Ui {
class StartMotorDialog;
}

class StartMotorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartMotorDialog(MSData &data, QWidget *parent = 0);
    ~StartMotorDialog();
    MSData data() const{return settingData;}

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::StartMotorDialog *ui;
    void dialogInit();
    void createConnection();
    MSData settingData;
    bool abletoclose;
    void setData(MSData data);
private slots:
    void checkBoxClicked();
    void lineEditChanged(QString str);
    void sliderChanged(int value);
    void okButtonClicked();
    void cancleButtonClicked();
};

#endif // STARTMOTORDIALOG_H
