#include "decisionbasedialog.h"
#include "ui_decisionbasedialog.h"

#include "dialogs/doctypedialog.h"
#include "dialogs/recorddialog.h"
#include "dialogs/protocoldialog.h"

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

    delete m_decisionModel;
    delete m_authorityModel;
    delete m_doctypeModel;
}


void DecisionBaseDialog::initialize()
{
    m_decisionModel = new DecisionModel;

    m_authorityModel = new AuthorityFlatModel;
    m_authorityModel->select();
    ui->cB_authority->setModel(m_authorityModel);

    m_doctypeModel = new DocumentTypeModel;
    m_doctypeModel->select();
    ui->cB_doctype->setModel(m_doctypeModel);
    ui->cB_doctype->setModelColumn(1);

    connect(ui->pB_doctype, &QPushButton::clicked, this, [=] { openExternal(ui->cB_doctype, new DoctypeDialog, 0); });
    connect(ui->pB_record, &QPushButton::clicked, this, [=] { openExternal(ui->cB_record, new RecordDialog, 2); });
    connect(ui->pB_protocol, &QPushButton::clicked, this, [=] { openExternal(ui->cB_protocol, new ProtocolDialog, 2); });
}

void DecisionBaseDialog::openExternal(QComboBox *cb, ReferenceDialog *dialog, int col)
{
    ReferenceModel *model = dynamic_cast<ReferenceModel*>(cb->model());

    if(model) {
        dialog->setChoiceMode();
        int res = dialog->exec();

        model->select();

        if(res == RecordDialog::Accepted) {
            setChosenId(cb, dialog->currentChoice(), col);
        }
    }

    delete dialog;
}

bool DecisionBaseDialog::setChosenId(QComboBox *cb, int id, int column)
{
    QAbstractItemModel *model = cb->model();
    for (int i = 0; i < model->rowCount(); ++i) {
        qDebug() << model->index(i, column).data() << "==" << id;
        if(model->index(i, column).data().toInt() == id) {
            cb->setCurrentIndex(i);
            return true;
        }
    }

    return false;
}
