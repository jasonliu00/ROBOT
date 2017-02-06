#include "showdialog.h"
#include "ui_showdialog.h"

ShowDialog::ShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("显示模块"));
    this->setWindowFlags(Qt::Drawer);

    QString stylesheet("QDialog{background-color:bisque;}");
    setStyleSheet(stylesheet);

    QString lineEditStyleSheet("QLineEdit{background-color:white;}");
    setStyleSheet(lineEditStyleSheet);

    dialogInit();
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
}
