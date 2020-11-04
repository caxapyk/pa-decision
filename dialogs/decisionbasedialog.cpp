#include "decisionbasedialog.h"
#include "ui_decisionbasedialog.h"

#include <QDebug>

DecisionBaseDialog::DecisionBaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecisionBaseDialog)
{
    ui->setupUi(this);

    initialize();
}

DecisionBaseDialog::~DecisionBaseDialog()
{
    delete ui;
}


void DecisionBaseDialog::initialize()
{
    m_authorityModel = new AuthorityFlatModel;
    m_authorityModel->select();
    ui->cB_authority->setModel(m_authorityModel);

}
