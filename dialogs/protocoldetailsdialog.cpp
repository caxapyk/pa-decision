#include "protocoldetailsdialog.h"
#include "ui_protocoldetailsdialog.h"

#include "models/protocolmodel.h"

#include <QDebug>
#include <QDate>
#include <QPushButton>
#include <QMessageBox>

ProtocolDetailsDialog::ProtocolDetailsDialog(const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProtocolDetailsDialog)
{
    m_id = id;
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &ProtocolDetailsDialog::reject);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &ProtocolDetailsDialog::accept);
}

ProtocolDetailsDialog::~ProtocolDetailsDialog()
{
    delete ui;
    delete m_mapper;
}

int ProtocolDetailsDialog::exec()
{
    if(m_id.isValid()) {
        ProtocolModel model;
        model.setFilter("id=" + m_id.toString());
        model.select();

        if (model.rowCount() > 0) {
            setWindowTitle(tr("Edit protocol"));

            m_mapper = new QDataWidgetMapper;
            m_mapper->setModel(&model);
            m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

            m_mapper->addMapping(ui->lE_number, 2);
            m_mapper->addMapping(ui->sB_pages, 3);
            m_mapper->addMapping(ui->lE_title, 4);
            m_mapper->addMapping(ui->dE_date, 5);
            m_mapper->addMapping(ui->lE_comment, 6);
            m_mapper->setCurrentIndex(0);

            connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &ProtocolDetailsDialog::accept);
        }
    } else {
        setWindowTitle(tr("New protocol"));
        ui->dE_date->setDate(QDate::currentDate());
    }

    connect(ui->dE_date, &QDateEdit::dateChanged, this, [=] { dateChanged = true; });

    return QDialog::exec();
}

void ProtocolDetailsDialog::accept()
{
    bool invalid = ui->lE_title->text().isEmpty() || ui->lE_number->text().isEmpty();
    if(invalid) {
        QMessageBox::critical(this,
                tr("New protocol"),
                tr("Could not create new protocol. Fill required fields."),
                QMessageBox::Ok);

        return;
    }

    QDialog::accept();
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

QString ProtocolDetailsDialog::getNumber() const
{
    return ui->lE_number->text();
}

QDate ProtocolDetailsDialog::getDate() const
{
    return ui->dE_date->date();
}

int ProtocolDetailsDialog::getPages() const
{
    return ui->sB_pages->value();
}
QString ProtocolDetailsDialog::getName() const

{
    return ui->lE_title->text();
}
QString ProtocolDetailsDialog::getComment() const
{
    return  ui->lE_comment->text();
}

