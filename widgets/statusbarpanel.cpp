#include "statusbarpanel.h"
#include "ui_statusbarpanel.h"

#include "application.h"

StatusBarPanel::StatusBarPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarPanel)
{
    ui->setupUi(this);
    ui->label_server->setText(application->server_address);
    ui->label_user->setText(application->server_username);
}

StatusBarPanel::~StatusBarPanel()
{
    delete ui;
}
