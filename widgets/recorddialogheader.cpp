#include "recorddialogheader.h"
#include "ui_recorddialogheader.h"

RecordDialogHeader::RecordDialogHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordDialogHeader)
{
    ui->setupUi(this);
}

RecordDialogHeader::~RecordDialogHeader()
{
    delete ui;
}

void RecordDialogHeader::setFundName(const QString &name)
{
    ui->label_fundName->setText(name);
}
