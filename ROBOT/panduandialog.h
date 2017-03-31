#ifndef PANDUANDIALOG_H
#define PANDUANDIALOG_H
#include "modelpropertydata.h"
#include <QDialog>
#include <QMap>

class QComboBox;
namespace Ui {
class PanduanDialog;
}

class PanduanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PanduanDialog(PdData panduandata, QWidget *parent = 0);
    PdData data() const{return mydata;}
    ~PanduanDialog();

private:
    Ui::PanduanDialog *ui;
    PdData mydata;
//    QMap<QString, QString> sensorIcon;
//    void sensorIconInit();
    void dialogInit();
    void createConnection();
    void setDialogOperatorRull(QString &type, QComboBox *comboBox);
    bool isCompareConditionValide(QString &type, QString &condition);
private slots:
    void checkBoxClicked(bool ischecked);
    void okButtonClicked();
    void cancelButtonClicked();
    void variable1Changed(QString name);
    void variable2Changed(QString name);
};

#endif // PANDUANDIALOG_H
