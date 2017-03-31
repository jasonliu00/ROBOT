#include "panduandialog.h"
#include "ui_panduandialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>

PanduanDialog::PanduanDialog(PdData panduandata, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PanduanDialog)
{
//    sensorIconInit();
    mydata.setData(panduandata);
    ui->setupUi(this);
    this->setWindowTitle(tr("判断模块"));
    this->setWindowFlags(Qt::Drawer);

    QString stylesheet("QDialog{background-color:bisque;}");
    setStyleSheet(stylesheet);

    QString lineEditStyleSheet("QLineEdit{background-color:white;}");
    setStyleSheet(lineEditStyleSheet);

    QString tabWigetStyleSheet("background-color:bisque");
    ui->tabWidget->setStyleSheet(tabWigetStyleSheet);
    dialogInit();
    createConnection();

}

PanduanDialog::~PanduanDialog()
{
    delete ui;
}

//void PanduanDialog::sensorIconInit()
//{
//    sensorIcon["AN_S"] = ":/images/dlgicon/AN_S.PNG";
//    sensorIcon["COUNTER_S"] = ":images/dlgicon/COUNTER_S.PNG";
//    sensorIcon["CSB_S"] = ":images/dlgicon/CSB_S.PNG";
//    sensorIcon["GZ_S"] = ":images/dlgicon/GZ_S.PNG";
//    sensorIcon["HY_S"] = ":images/dlgicon/HY_S.PNG";
//    sensorIcon["KQ_S"] = ":images/dlgicon/KQ_S.PNG";
//    sensorIcon["BZ_S"] = ":images/dlgicon/BZ_S.PNG";
//    sensorIcon["SY_S"] = ":images/dlgicon/SY_S.PNG";
//    sensorIcon["WSD_S"] = ":images/dlgicon/WSD_S.PNG";
//    sensorIcon["XJ_S"] = ":images/dlgicon/XJ_S.PNG";
//    sensorIcon["ZL_S"] = ":images/dlgicon/ZL_S.PNG";
//}

void PanduanDialog::dialogInit()
{
    QSqlQuery query;
    query.exec("SELECT name FROM sensorvariable;");
    ui->comboBox1_VariableName->setIconSize(QSize(32,32));
    ui->comboBox2_VariableName->setIconSize(QSize(32,32));
    while(query.next()){
        QString name = query.value(0).toString();
        ui->comboBox1_VariableName->addItem(QIcon(sensorIconMap.value(name)), name);
        ui->comboBox2_VariableName->addItem(QIcon(sensorIconMap.value(name)), name);
    }

    ui->tabWidget->setCurrentIndex(0);

    QPixmap pixmap(":/images/panduandlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);
    mydata.tj1_variableName = ui->comboBox1_VariableName->currentText();
    ui->comboBox1_VariableName->setCurrentText(mydata.tj1_variableName);
    ui->comboBox1_Operator->setCurrentText(mydata.tj1_operator);
    setDialogOperatorRull(mydata.tj1_variableName, ui->comboBox1_Operator);
    ui->lineEdit1_CompareContent->setText(mydata.tj1_compareContent);
    ui->checkBox->setChecked(mydata.tj2_isEnable);
    if(mydata.tj2_isEnable){
        mydata.tj2_variableName = ui->comboBox2_VariableName->currentText();
        ui->comboBox2_VariableName->setCurrentText(mydata.tj2_variableName);
        ui->comboBox2_Operator->setCurrentText(mydata.tj2_operator);
        setDialogOperatorRull(mydata.tj2_variableName, ui->comboBox2_VariableName);
        ui->lineEdit2_CompareContet->setText(mydata.tj2_compareContent);
        ui->comboBox2_Logic->setCurrentText(mydata.tj2_logicVersusTj1);
    }else{
        ui->comboBox2_VariableName->setEnabled(false);
        ui->comboBox2_Operator->setEnabled(false);
        ui->comboBox2_Logic->setEnabled(false);
        ui->lineEdit2_CompareContet->setEnabled(false);
    }
}

void PanduanDialog::createConnection()
{
    connect(ui->checkBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked(bool)));
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
    connect(ui->comboBox1_VariableName, SIGNAL(currentTextChanged(QString)),
            this, SLOT(variable1Changed(QString)));
    connect(ui->comboBox2_VariableName, SIGNAL(currentTextChanged(QString)),
            this, SLOT(variable2Changed(QString)));
}
/*
 * 根据下位机硬件的需求，设置一些判断的规则*/
void PanduanDialog::setDialogOperatorRull(QString &type, QComboBox *comboBox)
{
    if(type == "HY_S" || type == "SY_S" || type == "BZ_S"
            || type == "KQ_S" || type == "XJ_S"){
        comboBox->setCurrentText("==");
        comboBox->setDisabled(true);
    }else
        comboBox->setDisabled(false);
}

bool PanduanDialog::isCompareConditionValide(QString &type, QString &condition)
{
    if(type == "HY_S" || type == "SY_S"
            || type == "BZ_S" || type == "KQ_S"){
        if(condition != "1" && condition != "0"){
            QMessageBox::critical(this, tr("条件不符合规则"), tr("条件内容必须为0或1， 0表示没有检测到，1表示检测到"));
            return false;
        }else
            return true;
    }else if(type == "XJ_S"){
        if(condition != "1" && condition != "0"
                && condition != "2"){
            QMessageBox::critical(this, tr("条件不符合规则"), tr("条件内容必须为0、1或2, 0表示小车直行， 1表示小车右转，2表示小车左转"));
            return false;
        }else
            return true;
    }else{
        bool ok;
        int val = condition.toInt(&ok);
        if(ok == false){
            QMessageBox::critical(this, tr("条件不符合规则"), tr("条件内容必须为整数"));
            return false;
        }
        if(type == "WD_S"){
            if(val < 20 || val >50){
                QMessageBox::critical(this, tr("条件不符合规则"), tr("条件内容必须为20~50之间的整数,因为温度传感器的检测范围为20~50摄氏度"));
                return false;
            }else
                return true;
        }else if(type == "DH_S"){
            if(val < 20 || val > 90){
                QMessageBox::critical(this, tr("条件不符合规则"),
                                      tr("条件内容必须为20~90之间的整数,因为湿度传感器的检测范围为20~90,即相对湿度范围为20%RH~90%RH"));
                return false;
            }else
                return true;
        }else if(type == "GZ_S"){
            if(val < 0 || val >65535){
                QMessageBox::critical(this, tr("条件不符合规则"), tr("条件内容必须为0~65535之间的整数,因为光照传感器的检测范围为0~65535lx"));
                return false;
            }else
                return true;
        }else if(type == "CS_S"){
            if(val < 0 || val > 400){
                QMessageBox::critical(this, tr("条件不符合规则"), tr("条件内容必须为0~400之间的整数，因为超声波测距传感器的检测范围为0~400cm"));
                return false;
            }else
                return true;
        }
    }
    QMessageBox::critical(this, tr("未知错误"), tr("未知错误，请联系技术支持"));
    return false;
}

void PanduanDialog::checkBoxClicked(bool ischecked)
{
    ui->comboBox2_VariableName->setEnabled(ischecked);
    ui->comboBox2_Operator->setEnabled(ischecked);
    ui->comboBox2_Logic->setEnabled(ischecked);
    ui->lineEdit2_CompareContet->setEnabled(ischecked);
}

void PanduanDialog::okButtonClicked()
{
    mydata.tj1_variableName = ui->comboBox1_VariableName->currentText();
    mydata.tj1_operator = ui->comboBox1_Operator->currentText();
    mydata.tj1_compareContent = ui->lineEdit1_CompareContent->text();
    if(mydata.tj1_compareContent == ""){
        QMessageBox::warning(this, tr("warning"), tr("请输入比较条件！"));
        return;
    }else if(!isCompareConditionValide(mydata.tj1_variableName, mydata.tj1_compareContent))
        return;
    mydata.tj2_isEnable = ui->checkBox->isChecked();
    if(ui->checkBox->isChecked()){
        mydata.tj2_variableName = ui->comboBox2_VariableName->currentText();
        mydata.tj2_operator = ui->comboBox2_Operator->currentText();
        mydata.tj2_compareContent = ui->lineEdit2_CompareContet->text();
        mydata.tj2_logicVersusTj1 = ui->comboBox2_Logic->currentText();
        if(mydata.tj2_compareContent == ""){
            QMessageBox::warning(this, tr("warning"), tr("请输入比较条件！"));
            return;
        }else if(!isCompareConditionValide(mydata.tj2_variableName, mydata.tj2_compareContent))
            return;
    }
    accept();
}

void PanduanDialog::cancelButtonClicked()
{
    reject();
}

void PanduanDialog::variable1Changed(QString name)
{
    setDialogOperatorRull(name, ui->comboBox1_Operator);
}

void PanduanDialog::variable2Changed(QString name)
{
    setDialogOperatorRull(name, ui->comboBox2_Operator);
}
