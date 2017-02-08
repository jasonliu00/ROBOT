#include "delaydialog.h"
#include "ui_delaydialog.h"
#include <QCloseEvent>
#include <QMessageBox>

DelayDialog::DelayDialog(double time, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelayDialog)
{
    delaytime = time;
    ui->setupUi(this);
    this->setWindowTitle(tr("延时模块"));
    this->setWindowFlags(Qt::Drawer);  //设置该flag之后可以去掉对话框的icon

    QString stylesheet("QDialog{background-color:bisque;}");
    setStyleSheet(stylesheet);

    QString lineEditStyleSheet("QLineEdit{background-color:white;}");
    setStyleSheet(lineEditStyleSheet);

    createConnection();
    dialogInit();
}

DelayDialog::~DelayDialog()
{
    delete ui;
}

void DelayDialog::dialogInit()
{
    QPixmap pixmap(":/images/delaydlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);

    ui->lineEdit->setText(QString("%1").arg(delaytime, 0, 'f', 3));
}

void DelayDialog::createConnection()
{
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
}

void DelayDialog::okButtonClicked()
{
    QString str = ui->lineEdit->text();
    bool ok;
    double time = str.toInt(&ok);
    if(!ok){
        QMessageBox::warning(this, tr("delayDialog"), tr("请输入数字"));
        return;
    }else if(time > 100.0 || time < 0.0){
        QMessageBox::warning(this, tr("delayDialog"), tr("请输入0-100以内的数字"));
        return;
    }else{
        setData(time);
        accept();
    }
}

void DelayDialog::cancelButtonClicked()
{
    reject();
}
