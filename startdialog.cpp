#include "startdialog.h"
#include "ui_startdialog.h"

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);
    ui->rd_1->setChecked(true);
}

StartDialog::~StartDialog()
{
    delete ui;
}

int StartDialog::getOrder() const
{
    if(ui->rd_1->isChecked()){
        return 1;
    }
    else
    {
        return 2;
    }
}

void StartDialog::on_bt_sure_clicked()
{
    close();
}
