#include "protocoldetailsdialog.h"
#include "ui_protocoldetailsdialog.h"

ProtocolDetailsDialog::ProtocolDetailsDialog(QVariant id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProtocolDetailsDialog)
{
    ui->setupUi(this);
}

ProtocolDetailsDialog::~ProtocolDetailsDialog()
{
    delete ui;
}
