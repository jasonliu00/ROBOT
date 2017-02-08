#include "showdialog.h"
#include "ui_showdialog.h"
#include <QMessageBox>

ShowDialog::ShowDialog(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowDialog)
{
    setData(str);
    ui->setupUi(this);
    this->setWindowTitle(tr("显示模块"));
    this->setWindowFlags(Qt::Drawer);

    QString stylesheet("QDialog{background-color:bisque;}");
    setStyleSheet(stylesheet);

    QString lineEditStyleSheet("QLineEdit{background-color:white;}");
    setStyleSheet(lineEditStyleSheet);

    dialogInit();
    createConnection();
}

ShowDialog::~ShowDialog()
{
    delete ui;
}

void ShowDialog::dialogInit()
{
    QPixmap pixmap(":/images/showdlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);

    QStringList linestrs = showstr.split("\\n", QString::KeepEmptyParts, Qt::CaseSensitive);
    if(linestrs.count() != 7){
        QMessageBox::warning(this, tr("showdialog"), tr("string split error!"));
        return;
    }
    ui->lineEdit1->setText(linestrs.at(0));
    ui->lineEdit2->setText(linestrs.at(1));
    ui->lineEdit3->setText(linestrs.at(2));
    ui->lineEdit4->setText(linestrs.at(3));
    ui->lineEdit5->setText(linestrs.at(4));
    ui->lineEdit6->setText(linestrs.at(5));
    ui->lineEdit7->setText(linestrs.at(6));
}

void ShowDialog::createConnection()
{
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
}

void ShowDialog::okButtonClicked()
{
    setData("");
    QVector<QLineEdit*> namelist;
    namelist.reserve(7);
    namelist << ui->lineEdit1 << ui->lineEdit2 << ui->lineEdit3 << ui->lineEdit4
             << ui->lineEdit5 << ui->lineEdit6 << ui->lineEdit7;
    for(int i = 0; i < 7; i++){
        QString strtemp = namelist.at(i)->text();
        showstr.append(strtemp);
        showstr.append("\\n");
    }
    showstr.remove(showstr.count() - 2, 2);   //移除最后一个换行符，因为最后一行不需要换行
    accept();
}

void ShowDialog::cancelButtonClicked()
{
    reject();
}
