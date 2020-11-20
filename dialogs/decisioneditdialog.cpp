#include "decisioneditdialog.h"
#include "ui_decisionbasedialog.h"

#include <QDebug>

DecisionEditDialog::DecisionEditDialog(QWidget *parent) :
    DecisionBaseDialog(parent)
{
    setWindowTitle(tr("Edit decision"));

    ui->cB_authority->setDisabled(true);

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

DecisionEditDialog::~DecisionEditDialog()
{
    delete m_mapper;
}

int DecisionEditDialog::exec()
{
    if(model() != nullptr) {
        m_mapper->setModel(model());

        m_mapper->addMapping(ui->cB_record, 1);
        m_mapper->addMapping(ui->cB_authority, 12);
        m_mapper->addMapping(ui->cB_doctype, 3);
        m_mapper->addMapping(ui->cB_protocol, 13);
        m_mapper->addMapping(ui->dE_date, 5);
        m_mapper->addMapping(ui->lE_number, 6);
        m_mapper->addMapping(ui->lE_title, 7);
        m_mapper->addMapping(ui->cB_access, 8);
        m_mapper->addMapping(ui->tE_content, 9);
        m_mapper->addMapping(ui->lE_comment, 10);

        m_mapper->setCurrentIndex(0);

        if(model()->index(0, 4).data().isValid()) {
            ui->gB_protocol->setChecked(true);
        }
    }

    return DecisionBaseDialog::exec();
}
