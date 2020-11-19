#include "decisionbasedialog.h"
#include "ui_decisionbasedialog.h"

#include "dialogs/doctypedialog.h"
#include "dialogs/recorddialog.h"
#include "dialogs/protocoldialog.h"

#include <QDate>
#include <QDebug>

DecisionBaseDialog::DecisionBaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecisionBaseDialog)
{
    ui->setupUi(this);

    ui->dE_date->setDate(QDate::currentDate());
    initialize();
}

DecisionBaseDialog::~DecisionBaseDialog()
{
    delete ui;

    //delete m_decisionModel;
    delete m_authorityModel;
    delete m_doctypeModel;
    delete m_protocolModel;
    delete m_recordModel;
}


void DecisionBaseDialog::initialize()
{
    //m_decisionModel = new DecisionModel;

    m_doctypeModel = new DocumentTypeModel;
    m_doctypeModel->select();
    ui->cB_doctype->setModel(m_doctypeModel);
    ui->cB_doctype->setModelColumn(1);

    m_recordModel = new RecordFlatModel;
    m_protocolModel = new ProtocolFlatModel;

    m_authorityModel = new AuthorityFlatModel;
    ui->cB_authority->setModel(m_authorityModel);

    connect(ui->cB_authority, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DecisionBaseDialog::authorityChanged);
    connect(ui->cB_access, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DecisionBaseDialog::accessChanged);

    connect(ui->pB_doctype, &QPushButton::clicked, this, &DecisionBaseDialog::openDoctypeDialog);
    connect(ui->pB_protocol, &QPushButton::clicked, this,  &DecisionBaseDialog::openProtocolDialog);
    connect(ui->pB_record, &QPushButton::clicked, this, &DecisionBaseDialog::openRecordDialog);

    connect(ui->gB_protocol, &QGroupBox::toggled, this, &DecisionBaseDialog::protocolStateChanged);

    m_authorityModel->select();
}

void DecisionBaseDialog::authorityChanged(int index)
{
    m_authorityId = m_authorityModel->index(index, 2).data();

    if(m_authorityId.isValid()) {
        // load protocol
        if(ui->gB_protocol->isChecked()) {

            m_protocolModel->setAuthorityId(m_authorityId.toInt());
            m_protocolModel->select();

            ui->cB_protocol->setModel(m_protocolModel);
            ui->cB_protocol->setModelColumn(1);
        }

        // load record
        m_recordModel->setAuthorityId(m_authorityId.toInt());
        m_recordModel->select();
        ui->cB_record->setModel(m_recordModel);
        ui->cB_record->setModelColumn(1);
    }
}

void DecisionBaseDialog::protocolStateChanged(bool on)
{
    if(on) {
        if(m_authorityId.isValid()) {

            m_protocolModel->setAuthorityId(m_authorityId.toInt());
            m_protocolModel->select();
            ui->cB_protocol->setModel(m_protocolModel);
            ui->cB_protocol->setModelColumn(1);

            ui->cB_protocol->setEnabled(true);
        }
    } else {
        m_protocolModel->clear();
        ui->cB_protocol->setModel(m_protocolModel);

        m_recordModel->clear();
        ui->cB_record->setModel(m_recordModel);
    }
}

void DecisionBaseDialog::accessChanged(int index)
{
     ui->cB_access->setStyleSheet(QString("background-color:%1;").arg(index ? "green" : "red"));
}

void DecisionBaseDialog::openDoctypeDialog()
{
    DoctypeDialog dialog;
    openExternalDialog(ui->cB_doctype, &dialog);
}

void DecisionBaseDialog::openProtocolDialog()
{
    ProtocolDialog dialog;
    dialog.setAuthorityId(m_authorityId.toInt());

    openExternalDialog(ui->cB_protocol, &dialog);
}


void DecisionBaseDialog::openRecordDialog()
{
    RecordDialog dialog;
    dialog.setAuthorityId(m_authorityId.toInt());

    openExternalDialog(ui->cB_record, &dialog);
}


void DecisionBaseDialog::openExternalDialog(QComboBox *cb, ReferenceDialog *dialog)
{
    ReferenceModel *model = dynamic_cast<ReferenceModel*>(cb->model());

    if(model) {
        dialog->setChoiceMode();
        int res = dialog->exec();

        model->select();

        if(res == RecordDialog::Accepted) {
            setChosenId(cb, dialog->currentChoice());
        }
    }
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
