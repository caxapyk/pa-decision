#include "recorddetailsdialog.h"
#include "ui_recorddetailsdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>

RecordDetailsDialog::RecordDetailsDialog(QVariant id, QWidget *parent) :
    DetailsDialog(parent),
    ui(new Ui::RecordDetailsDialog)
{
    ui->setupUi(this);

    m_model = new QSqlTableModel;
    m_model->setTable("pad_fund");
    m_model->setFilter("id=" + id.toString());
    m_model->select();

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_mapper->setModel(m_model);
    m_mapper->addMapping(ui->lE_fundName, 3);
    m_mapper->toFirst();

    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
        if(m_mapper->submit()) {
            accept();
        } else {
            qDebug() << m_model->lastError().text();
            QMessageBox::warning(this,
                                 tr("Record details"),
                                 tr("Could not save data."),
                                 QMessageBox::Ok);
        }
    });

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &RecordDetailsDialog::reject);
}

RecordDetailsDialog::~RecordDetailsDialog()
{
    delete ui;
    delete m_model;
    delete m_mapper;
}

QString RecordDetailsDialog::getFundName() const
{
    return ui->lE_fundName->text();
}

void RecordDetailsDialog::reject()
{
    if(ui->lE_fundName->isModified()) {
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
