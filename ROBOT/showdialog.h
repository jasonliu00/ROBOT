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
    explicit ShowDialog(QString str, QWidget *parent = 0);
    ~ShowDialog();
    QString data()const{return showstr;}
    void setData(QString str){showstr = str;}
private:
    Ui::ShowDialog *ui;
    QString showstr;
    void dialogInit();
    void createConnection();
private slots:
    void okButtonClicked();
    void cancelButtonClicked();
};

#endif // SHOWDIALOG_H
