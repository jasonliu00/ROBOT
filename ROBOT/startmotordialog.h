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
    explicit StartMotorDialog(MStartData &data, QWidget *parent = 0);
    ~StartMotorDialog();
    MStartData data() const{return settingData;}

protected:
//    void closeEvent(QCloseEvent *event);

private:
    Ui::StartMotorDialog *ui;
    void dialogInit();
    void createConnection();
    MStartData settingData;
//    bool abletoclose;
    void setData(MStartData data);
private slots:
    void checkBoxClicked();
    void lineEditChanged(QString str);
    void sliderChanged(int value);
    void okButtonClicked();
    void cancleButtonClicked();
};

#endif // STARTMOTORDIALOG_H
