#include "funddetailsdialog.h"
#include "ui_funddetailsdialog.h"

FundDetailsDialog::FundDetailsDialog(QWidget *parent) :
    ReferenceDialog(parent),
    ui(new Ui::FundDetailsDialog)
{
    ui->setupUi(this);
}

FundDetailsDialog::~FundDetailsDialog()
{
    delete ui;
}
