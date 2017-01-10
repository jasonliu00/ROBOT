#include "startmotordialog.h"
#include "ui_startmotordialog.h"
#include <QMessageBox>
#include <QCloseEvent>

//bool StartMotorDialog::motorChecked[] = {false, false, false, false};
//int StartMotorDialog::motorPower[] = {80, 80, 80, 80};

StartMotorDialog::StartMotorDialog(MSData &data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartMotorDialog)
{
    setData(data);
    abletoclose = true;
    ui->setupUi(this);

    dialogInit();
    createConnection();
    QString stylesheet("background-color:bisque;");

    setStyleSheet(stylesheet);
}

StartMotorDialog::~StartMotorDialog()
{
    delete ui;
}

void StartMotorDialog::dialogInit()
{
    QPixmap pixmap(":/images/MStartDlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);
    ui->motor0CheckBox->setCheckable(true);
    ui->motor1CheckBox->setCheckable(true);
    ui->motor2CheckBox->setCheckable(true);
    ui->motor3CheckBox->setCheckable(true);

    ui->motor0CheckBox->setChecked(settingData.motorChecked[0]);
    ui->motor1CheckBox->setChecked(settingData.motorChecked[1]);
    ui->motor2CheckBox->setChecked(settingData.motorChecked[2]);
    ui->motor3CheckBox->setChecked(settingData.motorChecked[3]);

    QString stylesheet_edit("background-color:white;");
    ui->motor0LineEdit->setStyleSheet(stylesheet_edit);
    ui->motor1LineEdit->setStyleSheet(stylesheet_edit);
    ui->motor2LineEdit->setStyleSheet(stylesheet_edit);
    ui->motor3LineEdit->setStyleSheet(stylesheet_edit);

    ui->motor0LineEdit->setText(QString::number(settingData.motorPower[0]));
    ui->motor1LineEdit->setText(QString::number(settingData.motorPower[1]));
    ui->motor2LineEdit->setText(QString::number(settingData.motorPower[2]));
    ui->motor3LineEdit->setText(QString::number(settingData.motorPower[3]));

    ui->motor0LineEdit->setEnabled(settingData.motorChecked[0]);
    ui->motor1LineEdit->setEnabled(settingData.motorChecked[1]);
    ui->motor2LineEdit->setEnabled(settingData.motorChecked[2]);
    ui->motor3LineEdit->setEnabled(settingData.motorChecked[3]);


    ui->motor0Slider->setRange(-100, 100);
    ui->motor1Slider->setRange(-100, 100);
    ui->motor2Slider->setRange(-100, 100);
    ui->motor3Slider->setRange(-100, 100);

    ui->motor0Slider->setValue(settingData.motorPower[0]);
    ui->motor1Slider->setValue(settingData.motorPower[1]);
    ui->motor2Slider->setValue(settingData.motorPower[2]);
    ui->motor3Slider->setValue(settingData.motorPower[3]);

    ui->motor0Slider->setEnabled(settingData.motorChecked[0]);
    ui->motor1Slider->setEnabled(settingData.motorChecked[1]);
    ui->motor2Slider->setEnabled(settingData.motorChecked[2]);
    ui->motor3Slider->setEnabled(settingData.motorChecked[3]);

}

void StartMotorDialog::createConnection()
{
    connect(ui->motor0CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor1CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor2CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor3CheckBox, SIGNAL(clicked(bool)), this, SLOT(checkBoxClicked()));
    connect(ui->motor0LineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged(QString)));
    connect(ui->motor1LineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged(QString)));
    connect(ui->motor2LineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged(QString)));
    connect(ui->motor3LineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged(QString)));
    connect(ui->motor0Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(ui->motor1Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(ui->motor2Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(ui->motor3Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancleButton, SIGNAL(clicked(bool)), this, SLOT(cancleButtonClicked()));

}

void StartMotorDialog::setData(MSData data)
{
    int num = data.num;
    for(int i = 0; i < num; i++){
        settingData.motorChecked[i] = data.motorChecked[i];
        settingData.motorPower[i] = data.motorPower[i];
    }
}

void StartMotorDialog::checkBoxClicked()
{
    bool ischecked;
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if(checkBox == ui->motor0CheckBox){
        ischecked = ui->motor0CheckBox->isChecked();
        ui->motor0LineEdit->setEnabled(ischecked);
        ui->motor0Slider->setEnabled(ischecked);
    }
    if(checkBox == ui->motor1CheckBox){
        ischecked = ui->motor1CheckBox->isChecked();
        ui->motor1LineEdit->setEnabled(ischecked);
        ui->motor1Slider->setEnabled(ischecked);
    }
    if(checkBox == ui->motor2CheckBox){
        ischecked = ui->motor2CheckBox->isChecked();
        ui->motor2LineEdit->setEnabled(ischecked);
        ui->motor2Slider->setEnabled(ischecked);
    }
    if(checkBox == ui->motor3CheckBox){
        ischecked = ui->motor3CheckBox->isChecked();
        ui->motor3LineEdit->setEnabled(ischecked);
        ui->motor3Slider->setEnabled(ischecked);
    }
}

void StartMotorDialog::lineEditChanged(QString str)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
//    QString str = lineEdit->text();
/****输入的内容只能是-100~100以内的正数，此处判断仍有问题****/
    bool ok;
    int num = str.toInt(&ok);
    if(!ok){
//        QMessageBox::warning(this, tr("warning"), tr("请输入-100~100的整数"));
//        lineEdit->setText("");
        return;
    }else if(num < -100 || num > 100){
//        QMessageBox::warning(this, tr("warning"), tr("请输入-100~100的整数"));
//        lineEdit->setText("");
        return;
    }
/*****************************************************/
    if(lineEdit == ui->motor0LineEdit){
//        motorPower[0] = num;
        ui->motor0Slider->setValue(num);
    }
    if(lineEdit == ui->motor1LineEdit){
//        motorPower[1] = num;
        ui->motor1Slider->setValue(num);
    }
    if(lineEdit == ui->motor2LineEdit){
//        motorPower[2] = num;
        ui->motor2Slider->setValue(num);
    }
    if(lineEdit == ui->motor3LineEdit){
//        motorPower[3] = num;
        ui->motor3Slider->setValue(num);
    }
}

void StartMotorDialog::sliderChanged(int value)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    if(slider == ui->motor0Slider){
//        motorPower[0] = value;
        ui->motor0LineEdit->setText(QString::number(value));
    }
    if(slider == ui->motor1Slider){
//        motorPower[1] = value;
        ui->motor1LineEdit->setText(QString::number(value));
    }
    if(slider == ui->motor2Slider){
//        motorPower[2] = value;
        ui->motor2LineEdit->setText(QString::number(value));
    }
    if(slider == ui->motor3Slider){
//        motorPower[3] = value;
        ui->motor3LineEdit->setText(QString::number(value));
    }
}

void StartMotorDialog::okButtonClicked()
{
    QVector<QLineEdit*> lineedits;
    lineedits << ui->motor0LineEdit << ui->motor1LineEdit
              << ui->motor2LineEdit << ui->motor3LineEdit;
    bool ok;
    foreach(QLineEdit* edit, lineedits){
        int value = edit->text().toInt(&ok);
        if(!ok){
            QMessageBox::warning(this, tr("warning"), tr("请输入-100~100的整数"));
            edit->setFocus();
            abletoclose = false;
            return;
        }
        if(value < -100 || value >100){
            QMessageBox::warning(this, tr("warning"), tr("请输入-100~100的整数"));
            edit->setFocus();
            abletoclose = false;
            return;
        }
    }

    settingData.motorPower[0] = ui->motor0Slider->value();
    settingData.motorPower[1] = ui->motor1Slider->value();
    settingData.motorPower[2] = ui->motor2Slider->value();
    settingData.motorPower[3] = ui->motor3Slider->value();

    settingData.motorChecked[0] = ui->motor0CheckBox->isChecked();
    settingData.motorChecked[1] = ui->motor1CheckBox->isChecked();
    settingData.motorChecked[2] = ui->motor2CheckBox->isChecked();
    settingData.motorChecked[3] = ui->motor3CheckBox->isChecked();

    accept();
}

void StartMotorDialog::closeEvent(QCloseEvent *event)
{
    if(abletoclose)
        event->accept();
    else{
        event->ignore();
        abletoclose = true;
    }
}

void StartMotorDialog::cancleButtonClicked()
{
    accept();
}
