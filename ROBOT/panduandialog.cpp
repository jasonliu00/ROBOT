#include "panduandialog.h"
#include "ui_panduandialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

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

    ui->comboBox1_VariableName->setCurrentText(mydata.tj1_variableName);
    ui->comboBox1_Operator->setCurrentText(mydata.tj1_operator);
    ui->lineEdit1_CompareContent->setText(mydata.tj1_compareContent);
    ui->checkBox->setChecked(mydata.tj2_isEnable);
    if(mydata.tj2_isEnable){
        ui->comboBox2_VariableName->setCurrentText(mydata.tj2_variableName);
        ui->comboBox2_Operator->setCurrentText(mydata.tj2_operator);
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
    }
    mydata.tj2_isEnable = ui->checkBox->isChecked();
    if(ui->checkBox->isChecked()){
        mydata.tj2_variableName = ui->comboBox2_VariableName->currentText();
        mydata.tj2_operator = ui->comboBox2_Operator->currentText();
        mydata.tj2_compareContent = ui->lineEdit2_CompareContet->text();
        mydata.tj2_logicVersusTj1 = ui->comboBox2_Logic->currentText();
        if(mydata.tj2_compareContent == ""){
            QMessageBox::warning(this, tr("warning"), tr("请输入比较条件！"));
            return;
        }
    }
    accept();
}

void PanduanDialog::cancelButtonClicked()
{
    reject();
}
