#include "lightdialog.h"
#include "ui_lightdialog.h"

LightDialog::LightDialog(bool state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LightDialog)
{
    lightstate = state;
    ui->setupUi(this);
    this->setWindowTitle(tr("指示灯模块"));
    this->setWindowFlags(Qt::Drawer);

    QString stylesheet("background-color:bisque;");
    setStyleSheet(stylesheet);

    createConnection();
    dialogInit();
}

LightDialog::~LightDialog()
{
    delete ui;
}

void LightDialog::dialogInit()
{
    QPixmap pixmap(":/images/lightdlg.PNG");
    ui->pictureLabel->resize(pixmap.size());
    ui->pictureLabel->setPixmap(pixmap);

    if(lightstate)
        ui->onRadioButton->setChecked(true);
    else
        ui->offRadioButton->setChecked(true);
}

void LightDialog::createConnection()
{
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked()));
}

void LightDialog::okButtonClicked()
{
    if(ui->onRadioButton->isChecked())
        setData(true);
    else
        setData(false);
    accept();
}

void LightDialog::cancelButtonClicked()
{
    reject();
}
