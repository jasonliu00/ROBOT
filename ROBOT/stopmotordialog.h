#ifndef STOPMOTORDIALOG_H
#define STOPMOTORDIALOG_H

#include <QDialog>

namespace Ui {
class StopMotorDialog;
}

class StopMotorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StopMotorDialog(QWidget *parent = 0);
    ~StopMotorDialog();

private:
    Ui::StopMotorDialog *ui;
    static bool motorStop[4];
    void dialogInit();
    void createConnection();
private slots:
    void checkBoxClicked();
    void okButtonClicked();
    void cancelButtonClicked();
};

#endif // STOPMOTORDIALOG_H
