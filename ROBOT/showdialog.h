#ifndef SHOWDIALOG_H
#define SHOWDIALOG_H

#include <QDialog>

namespace Ui {
class ShowDialog;
}

class ShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowDialog(QWidget *parent = 0);
    ~ShowDialog();

private:
    Ui::ShowDialog *ui;

    void dialogInit();
};

#endif // SHOWDIALOG_H
