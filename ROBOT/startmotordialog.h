#ifndef STARTMOTORDIALOG_H
#define STARTMOTORDIALOG_H

#include <QDialog>

namespace Ui {
class StartMotorDialog;
}

class StartMotorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartMotorDialog(QWidget *parent = 0);
    ~StartMotorDialog();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::StartMotorDialog *ui;
    void dialogInit();
    void createConnection();
    static bool motorChecked[4];
    static int motorPower[4];
    bool abletoclose;
private slots:
    void checkBoxClicked();
    void lineEditChanged(QString str);
    void sliderChanged(int value);
    void okButtonClicked();
    void cancleButtonClicked();
};

#endif // STARTMOTORDIALOG_H
