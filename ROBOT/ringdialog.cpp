#include "ringdialog.h"
#include "ui_ringdialog.h"
#include <QMessageBox>
#include <QButtonGroup>

RingDialog::RingDialog(RingData ringdata, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RingDialog)
{
    ringsetting.setData(ringdata);
    ui->setupUi(this);
    this->setWindowTitle(tr("蜂鸣器模块"));
    this->setWindowFlags(Qt::Drawer);

    QString stylesheet("QDialog{background-color:bisque;}");
    setStyleSheet(stylesheet);

    QString lineEditStyleSheet("QLineEdit{background-color:white;}");
    setStyleSheet(lineEditStyleSheet);

    dialogInit();
    createConnection();
}

RingDialog::~RingDialog()
{
    delete ui;
}

void RingDialog::dialogInit()
{
    QPixmap pixmap(":/images/ringdlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);

    yinfuButtonGroup = new QButtonGroup();  //用buttonGroup方便管理
    yinfuButtonGroup->setExclusive(true);
    yinfuButtonGroup->addButton(ui->radioButton_21, 0);   //21代表2分之一,41代表4分之一
    yinfuButtonGroup->addButton(ui->radioButton_41, 1);
    yinfuButtonGroup->addButton(ui->radioButton_81, 2);
    yinfuButtonGroup->addButton(ui->radioButton_161, 3);

    yinpinButtonGroup = new QButtonGroup();
    yinpinButtonGroup->setExclusive(true);
    yinpinButtonGroup->addButton(ui->xiuzhifuRadioButton, 0);
    yinpinButtonGroup->addButton(ui->radioButton_G1, 1); //G1代表高音1,D1代表低音1
    yinpinButtonGroup->addButton(ui->radioButton_G2, 2);
    yinpinButtonGroup->addButton(ui->radioButton_G3, 3);
    yinpinButtonGroup->addButton(ui->radioButton_G4, 4);
    yinpinButtonGroup->addButton(ui->radioButton_G5, 5);
    yinpinButtonGroup->addButton(ui->radioButton_G6, 6);
    yinpinButtonGroup->addButton(ui->radioButton_G7, 7);

    yinpinButtonGroup->addButton(ui->radioButton_1, 8);
    yinpinButtonGroup->addButton(ui->radioButton_2, 9);
    yinpinButtonGroup->addButton(ui->radioButton_3, 10);
    yinpinButtonGroup->addButton(ui->radioButton_4, 11);
    yinpinButtonGroup->addButton(ui->radioButton_5, 12);
    yinpinButtonGroup->addButton(ui->radioButton_6, 13);
    yinpinButtonGroup->addButton(ui->radioButton_7, 14);

    yinpinButtonGroup->addButton(ui->radioButton_D1, 15);
    yinpinButtonGroup->addButton(ui->radioButton_D2, 16);
    yinpinButtonGroup->addButton(ui->radioButton_D3, 17);
    yinpinButtonGroup->addButton(ui->radioButton_D4, 18);
    yinpinButtonGroup->addButton(ui->radioButton_D5, 19);
    yinpinButtonGroup->addButton(ui->radioButton_D6, 20);
    yinpinButtonGroup->addButton(ui->radioButton_D7, 21);

    if(ringsetting.yinfuID != -1)
        yinfuButtonGroup->button(ringsetting.yinfuID)->setChecked(true);
    ui->timeLineEdit->setText(QString::number(ringsetting.yinfuTime, 'f', 4));
//    ui->timeLineEdit->setEnabled(false);   //暂时不开放用户自定义时间
    yinpinButtonGroup->button(ringsetting.yinpinID)->setChecked(true);
    ui->yinpinLineEdit->setText(QString::number(yinpin[ringsetting.yinpinID], 'f', 1));

}

void RingDialog::createConnection()
{
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
    connect(yinpinButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(yinpinBGClicked(int)));
    connect(yinfuButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(yinfuBGClicked(int)));
    connect(ui->timeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(timeTextChanged(QString)));
    connect(ui->yinpinLineEdit, SIGNAL(textChanged(QString)), this, SLOT(yinpinTextChanged(QString)));
}

void RingDialog::okButtonClicked()
{
   bool ok;
   float time;
   int ID = yinfuButtonGroup->checkedId();
   if(ID >= 0){
       ringsetting.yinfuTime = yinfu[ID];
       ringsetting.yinfuID = ID;
   }
   else{
       time = ui->timeLineEdit->text().toFloat(&ok);
       if(!ok){
           QMessageBox::warning(this, tr("ringdialog"), tr("时间必须是数字"));
           return;
       }else if(time > 10.0 || time < 0.0){
           QMessageBox::warning(this, tr("ringdialog"), tr("时间的范围为0-10"));
           return;
       }
       ringsetting.yinfuTime = time;
   }
   ringsetting.yinpinID = yinpinButtonGroup->checkedId();

   accept();
}

void RingDialog::cancelButtonClicked()
{
    reject();
}

void RingDialog::yinpinBGClicked(int Id)
{
    ui->yinpinLineEdit->setText(QString::number(yinpin[Id], 'f', 1));
}

void RingDialog::yinfuBGClicked(int Id)
{
    ui->timeLineEdit->setText(QString::number(yinfu[Id], 'f', 4));
}

void RingDialog::timeTextChanged(QString str)
{
    bool ok;
    float time = str.toFloat(&ok);
    if(ok){
       if(time == 0.5)
            ui->radioButton_21->setChecked(true);
       if(time == 0.25)
            ui->radioButton_41->setChecked(true);
       if(time == 0.125)
            ui->radioButton_81->setChecked(true);
       if(time == 0.0625)
            ui->radioButton_161->setChecked(true);
    }
}

void RingDialog::yinpinTextChanged(QString str)
{
    bool ok;
    bool flag = false;
    int i;
    float yp = str.toFloat(&ok);
    if(ok){
        for(i = 0; i < 22; i++){
            if(yp == yinpin[i]){
                flag = true;
                break;
            }
        }
        if(flag)
            yinpinButtonGroup->button(i)->setChecked(true);
    }
}
