#include "protocoldetailsdialog.h"
#include "ui_protocoldetailsdialog.h"

#include <QPushButton>
#include <QMessageBox>

ProtocolDetailsDialog::ProtocolDetailsDialog(ProtocolModel *model, int row, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ProtocolDetailsDialog)
{
    ui->setupUi(this);

    if(row >= 0) {
        setWindowTitle(tr("Edit protocol"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 0);
        m_mapper->addMapping(ui->dE_date, 1);
        m_mapper->addMapping(ui->lE_title, 2);
        m_mapper->addMapping(ui->lE_comment, 3);

        m_mapper->setCurrentIndex(row);

    } else {
        setWindowTitle(tr("New protocol"));
    }

    m_model = model;
    mrow = row;

    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &ProtocolDetailsDialog::accept);
    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &ProtocolDetailsDialog::reject);
}

ProtocolDetailsDialog::~ProtocolDetailsDialog()
{
    delete ui;
    delete m_mapper;
}

void ProtocolDetailsDialog::accept()
{
    if(ui->lE_number->text().isEmpty() || ui->lE_title->text().isEmpty()) {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("All required fields should be set."),
                QMessageBox::Ok);

        return;
    }

    prop.insert("authority_id", m_model->authorityId());
    prop.insert("number", ui->lE_number->text());
    prop.insert("date", ui->dE_date->text());
    prop.insert("title", ui->lE_title->text());
    prop.insert("comment", ui->lE_comment->text());

    if(mrow > 0) {
        if(!m_mapper->submit()) {
            QMessageBox::warning(this,
                    tr("Update items"),
                    tr("Could not save item."),
                    QMessageBox::Ok);

            return;
        }
    } else {
        bool insert = m_model->setRow(mrow, prop);

        if(!insert) {
            QMessageBox::warning(this,
                    tr("Creating items"),
                    tr("Could not create item."),
                    QMessageBox::Ok);

            return;
        }
    }

    QDialog::accept();
}

void ProtocolDetailsDialog::reject()
{
    if(m_model != nullptr) {
        m_mapper->revert();
    }

    QDialog::reject();
}
