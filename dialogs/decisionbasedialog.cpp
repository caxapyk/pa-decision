#include "decisionbasedialog.h"
#include "ui_decisionbasedialog.h"

DecisionBaseDialog::DecisionBaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecisionBaseDialog)
{
    ui->setupUi(this);
}

DecisionBaseDialog::~DecisionBaseDialog()
{
    delete ui;
}
