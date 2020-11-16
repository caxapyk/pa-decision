#include "recorddetailsdialog.h"
#include "ui_recorddetailsdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

RecordDetailsDialog::RecordDetailsDialog(QVariant id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDetailsDialog)
{
    ui->setupUi(this);

    m_model = new QSqlTableModel;
    m_model->setTable("pad_record");
    m_model->setFilter("id=" + id.toString());
    m_model->select();

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_mapper->setModel(m_model);
    m_mapper->addMapping(ui->lE_name, 3);
    m_mapper->toFirst();

    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
        m_mapper->submit();
    });
    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &RecordDetailsDialog::reject);
}

RecordDetailsDialog::~RecordDetailsDialog()
{
    delete ui;
    delete m_model;
    delete m_mapper;
}

void RecordDetailsDialog::reject()
{
    if(ui->lE_name->isModified()) {
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
