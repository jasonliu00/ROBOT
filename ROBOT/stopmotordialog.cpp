#include "stopmotordialog.h"
#include "ui_stopmotordialog.h"

bool StopMotorDialog::motorStop[] = {true, true, true, true};
StopMotorDialog::StopMotorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StopMotorDialog)
{
    ui->setupUi(this);
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
    ui->motor0CheckBox->setChecked(motorStop[0]);
    ui->motor1CheckBox->setChecked(motorStop[1]);
    ui->motor2CheckBox->setChecked(motorStop[2]);
    ui->motor3CheckBox->setChecked(motorStop[3]);

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
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancleButtonClicked()));
}

void StopMotorDialog::checkBoxClicked()
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
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
    motorStop[0] = ui->motor0CheckBox->isChecked();
    motorStop[1] = ui->motor1CheckBox->isChecked();
    motorStop[2] = ui->motor2CheckBox->isChecked();
    motorStop[3] = ui->motor3CheckBox->isChecked();

    accept();
}

void StopMotorDialog::cancelButtonClicked()
{
    accept();
}
