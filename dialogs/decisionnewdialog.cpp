#include "decisionnewdialog.h"
#include "ui_decisionbasedialog.h"

DecisionNewDialog::DecisionNewDialog(QWidget *parent) :
    DecisionBaseDialog(parent)
{
    setWindowTitle(tr("New decision"));
}

DecisionNewDialog::~DecisionNewDialog()
{
}

void DecisionNewDialog::setAuthorityId(int id)
{
    if(setChosenId(ui->cB_authority, id, 2)) {
        ui->cB_authority->setDisabled(true);
    }
}
