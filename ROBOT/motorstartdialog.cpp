#include "motorstartdialog.h"
#include "ui_motorstartdialog.h"

MotorStartDialog::MotorStartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotorStartDialog)
{
    ui->setupUi(this);
    ui->horizontalSlider_1->setRange(-100, 100);
    ui->horizontalSlider_2->setRange(-100, 100);
    ui->horizontalSlider_3->setRange(-100, 100);
    ui->horizontalSlider_4->setRange(-100, 100);

    ui->horizontalSlider_1->setValue(80);
    ui->horizontalSlider_2->setValue(80);
    ui->horizontalSlider_3->setValue(80);
    ui->horizontalSlider_4->setValue(80);

    ui->horizontalSlider_3->setEnabled(false);
    ui->horizontalSlider_4->setEnabled(false);

    ui->lineEdit->setText(tr("80"));
    ui->lineEdit_2->setText(tr("80"));
    ui->lineEdit_3->setText(tr("80"));
    ui->lineEdit_4->setText(tr("80"));

    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);

    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(true);
}

MotorStartDialog::~MotorStartDialog()
{
    delete ui;
}
