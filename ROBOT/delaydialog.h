#ifndef DELAYDIALOG_H
#define DELAYDIALOG_H

#include <QDialog>

namespace Ui {
class DelayDialog;
}

class DelayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DelayDialog(int time,QWidget *parent = 0);
    ~DelayDialog();
    int data()const{return delaytime;}
    void setData(double time){delaytime = time;}
protected:
//    void closeEvent(QCloseEvent *event);
private:
    Ui::DelayDialog *ui;

    int delaytime;
    void dialogInit();
    void createConnection();
private slots:
    void okButtonClicked();
    void cancelButtonClicked();

};

#endif // DELAYDIALOG_H
