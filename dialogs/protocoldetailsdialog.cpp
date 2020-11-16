#include "protocoldetailsdialog.h"
#include "ui_protocoldetailsdialog.h"

#include <QDebug>
#include <QDate>
#include <QPushButton>
#include <QMessageBox>

#include <QSqlError>

ProtocolDetailsDialog::ProtocolDetailsDialog(QVariant id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProtocolDetailsDialog)
{
    ui->setupUi(this);
    ui->dE_date->setDate(QDate::currentDate());

    if(id.isValid()) {
        setWindowTitle(tr("Edit protocol"));

        m_model = new QSqlTableModel;
        m_model->setTable("pad_protocol");
        m_model->setFilter("id=" + id.toString());
        m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        m_model->select();

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(m_model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_name, 4);
        m_mapper->addMapping(ui->lE_number, 2);
        m_mapper->addMapping(ui->dE_date, 3);

        m_mapper->toFirst();

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
            m_mapper->submit();
            m_model->submitAll();
        });

    } else {
        setWindowTitle(tr("New protocol"));

        m_model = new QSqlTableModel;
        m_model->setTable("pad_protocol");

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &ProtocolDetailsDialog::insert);
    }

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &ProtocolDetailsDialog::reject);
    connect(ui->dE_date, &QDateEdit::dateChanged, this, [=] { _form_changed = true; });
}

ProtocolDetailsDialog::~ProtocolDetailsDialog()
{
    delete ui;

    delete m_model;
    delete m_mapper;
}

void ProtocolDetailsDialog::insert()
{
    m_model->insertRow(0);
    m_model->setData(m_model->index(0,1), aid);
    m_model->setData(m_model->index(0,2), ui->lE_number->text());
    m_model->setData(m_model->index(0,3), ui->dE_date->date());
    m_model->setData(m_model->index(0,4), ui->lE_name->text());
    m_model->submitAll();
}

void ProtocolDetailsDialog::reject()
{
    bool commit = ui->lE_name->isModified() || ui->lE_number->isModified() || _form_changed;
    if(commit) {
    int res = QMessageBox::critical(this,
        tr("Close without saving"),
        tr("Are you shure that you want to dicard all changes?"),
        QMessageBox::No | QMessageBox::Yes);

        if (res == QMessageBox::Yes) {
            QDialog::reject();
        }
    } else {
        QDialog::reject();
    }
}
