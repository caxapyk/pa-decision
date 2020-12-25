#include "protocoldetailsdialog.h"
#include "ui_protocoldetailsdialog.h"

#include "models/standardreferencemodel.h"
#include "models/protocolmodel.h"

#include <QDebug>
#include <QDate>
#include <QPushButton>
#include <QMessageBox>

ProtocolDetailsDialog::ProtocolDetailsDialog(QWidget *parent) :
    DetailsDialog(parent),
    ui(new Ui::ProtocolDetailsDialog)
{
    ui->setupUi(this);
    ui->dE_date->setDate(QDate::currentDate());

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &ProtocolDetailsDialog::reject);
    connect(ui->dE_date, &QDateEdit::dateChanged, this, [=] { dateChanged = true; });
}

ProtocolDetailsDialog::~ProtocolDetailsDialog()
{
    delete ui;
    delete m_mapper;
}

int ProtocolDetailsDialog::exec()
{
    if (model()) {
        if(currentIndex().isValid()) {
            setWindowTitle(tr("Edit protocol"));

            m_mapper = new QDataWidgetMapper;
            m_mapper->setModel(model());
            m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

            m_mapper->addMapping(ui->lE_number, 2);
            m_mapper->addMapping(ui->sB_pages, 3);
            m_mapper->addMapping(ui->lE_title, 4);
            m_mapper->addMapping(ui->dE_date, 5);
            m_mapper->addMapping(ui->lE_comment, 6);
            m_mapper->setCurrentIndex(currentIndex().row());

            connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
                if(m_mapper->submit()) {
                    accept();
                } else {
                    QMessageBox::critical(this,
                            tr("Protocol"),
                            tr("Could not save protocol"),
                            QMessageBox::Ok);
                }
            });
        } else {
            setWindowTitle(tr("New protocol"));
            connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &ProtocolDetailsDialog::insert);
        }

        return DetailsDialog::exec();
    }

    return 1;
}

void ProtocolDetailsDialog::insert()
{
    bool invalid = ui->lE_title->text().isEmpty() || ui->lE_number->text().isEmpty();
    if(invalid) {
        QMessageBox::critical(this,
                tr("New protocol"),
                tr("Could not create new protocol. Fill required fields."),
                QMessageBox::Ok);

        return;
    }

    StandardReferenceModel *m = dynamic_cast<StandardReferenceModel*>(model());
    connect(m->sourceModel(), &QSqlRelationalTableModel::primeInsert, this, &ProtocolDetailsDialog::setRecord);

    if(!m->insertRow(0)) {
        QMessageBox::critical(this,
                tr("New protocol"),
                tr("Could not create new protocol"),
                QMessageBox::Ok);
    }

    accept();

}

void ProtocolDetailsDialog::setRecord(int, QSqlRecord &record)
{
    ProtocolModel *m = dynamic_cast<ProtocolModel*>(model());

    record.setValue("record_id", m->recordId());
    record.setGenerated("record_id", true);

    record.setValue("number", ui->lE_number->text());
    record.setGenerated("number", true);

    record.setValue("pages", ui->sB_pages->value());
    record.setGenerated("pages", true);

    record.setValue("name", ui->lE_title->text());
    record.setGenerated("name", true);

    record.setValue("date", ui->dE_date->date());
    record.setGenerated("date", true);

    record.setValue("comment", ui->lE_comment->text());
    record.setGenerated("comment", true);
}

void ProtocolDetailsDialog::reject()
{
    bool commit = ui->lE_title->isModified() || ui->lE_number->isModified() || dateChanged;
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
