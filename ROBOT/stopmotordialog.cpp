#include "stopmotordialog.h"
#include "ui_stopmotordialog.h"

StopMotorDialog::StopMotorDialog(MStopData &data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StopMotorDialog)
{
    settingData.setData(data);
    ui->setupUi(this);
    this->setWindowTitle(tr("马达停止模块"));
    this->setWindowFlags(Qt::Drawer);

    QString stylesheet("background-color:bisque;");
    setStyleSheet(stylesheet);
    dialogInit();
    createConnection();
}

StopMotorDialog::~StopMotorDialog()
{
    delete ui;
}

void StopMotorDialog::dialogInit()
{
    ui->motor0CheckBox->setChecked(settingData.motorChecked[0]);
    ui->motor1CheckBox->setChecked(settingData.motorChecked[1]);
    ui->motor2CheckBox->setChecked(settingData.motorChecked[2]);
    ui->motor3CheckBox->setChecked(settingData.motorChecked[3]);

    QPixmap pixmap(":/images/MStopDlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);
}

void StopMotorDialog::createConnection()
{
    connect(ui->motor0CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor1CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor2CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor3CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
}

void StopMotorDialog::checkBoxClicked()
{
//    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
//    if(checkBox == ui->motor0CheckBox);
//        motorStop[0] = checkBox->isChecked();
//    if(checkBox == ui->motor1CheckBox);
//        motorStop[1] = checkBox->isChecked();
//    if(checkBox == ui->motor2CheckBox);
//        motorStop[2] = checkBox->isChecked();
//    if(checkBox == ui->motor3CheckBox);
//        motorStop[3] = checkBox->isChecked();
}

void StopMotorDialog::okButtonClicked()
{
    settingData.motorChecked[0] = ui->motor0CheckBox->isChecked();
    settingData.motorChecked[1] = ui->motor1CheckBox->isChecked();
    settingData.motorChecked[2] = ui->motor2CheckBox->isChecked();
    settingData.motorChecked[3] = ui->motor3CheckBox->isChecked();

    accept();
}

void StopMotorDialog::cancelButtonClicked()
{
    reject();
}
