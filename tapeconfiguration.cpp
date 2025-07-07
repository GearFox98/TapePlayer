#include "tapeconfiguration.h"
#include "ui_tapeconfiguration.h"

TapeConfiguration::TapeConfiguration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TapeConfiguration)
{
    ui->setupUi(this);

    ui->cbNativeNotifications->setChecked(this->native);
    ui->cbNotify->setChecked(this->notify);
}

TapeConfiguration::~TapeConfiguration()
{
    delete ui;
}

void TapeConfiguration::on_btClose_clicked()
{
    this->close();
}

void TapeConfiguration::on_btApply_clicked()
{
    // TODO
}

void TapeConfiguration::on_tbBrowse_clicked()
{
    // TODO
}

