#ifndef MOTORSTARTDIALOG_H
#define MOTORSTARTDIALOG_H

#include <QDialog>

namespace Ui {
class MotorStartDialog;
}

class MotorStartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorStartDialog(QWidget *parent = 0);
    ~MotorStartDialog();

private:
    Ui::MotorStartDialog *ui;
};

#endif // MOTORSTARTDIALOG_H
