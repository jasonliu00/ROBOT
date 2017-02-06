#ifndef LIGHTDIALOG_H
#define LIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class LightDialog;
}

class LightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LightDialog(bool state, QWidget *parent = 0);
    ~LightDialog();
    bool data()const{return lightstate;}
    void setData(bool state){lightstate = state;}

private:
    Ui::LightDialog *ui;

    bool lightstate;
    void dialogInit();
    void createConnection();

private slots:
    void okButtonClicked();
    void cancelButtonClicked();
};

#endif // LIGHTDIALOG_H
