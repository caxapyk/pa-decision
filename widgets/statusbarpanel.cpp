#include "statusbarpanel.h"
#include "ui_statusbarpanel.h"

#include "application.h"

StatusBarPanel::StatusBarPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarPanel)
{
    ui->setupUi(this);
    ui->label_serverAddress->setText(application->server_address);
}

StatusBarPanel::~StatusBarPanel()
{
    delete ui;
}

void StatusBarPanel::setTotal(int total)
{
    ui->label_total->setText(tr("Total rows %1").arg(QString::number(total)));
}
