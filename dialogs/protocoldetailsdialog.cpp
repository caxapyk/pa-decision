#include "protocoldetailsdialog.h"
#include "ui_protocoldetailsdialog.h"

#include <QDate>
#include <QPushButton>
#include <QMessageBox>

ProtocolDetailsDialog::ProtocolDetailsDialog(int mode, ProtocolModel *model, int row, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ProtocolDetailsDialog)
{
    ui->setupUi(this);

    if(mode == UpdateMode) {
        setWindowTitle(tr("Edit protocol"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 0);
        m_mapper->addMapping(ui->dE_date, 1);
        m_mapper->addMapping(ui->lE_title, 2);
        m_mapper->addMapping(ui->lE_comment, 3);

        m_mapper->setCurrentIndex(row);

    } else if(mode == InsertMode) {
        setWindowTitle(tr("New protocol"));
    }

    m_model = model;
    mrow = row;
    mmode = mode;

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
    prop.insert("date", QDate().fromString(ui->dE_date->text(), "dd.MM.yyyy"));
    prop.insert("title", ui->lE_title->text());
    prop.insert("comment", ui->lE_comment->text());

    if(mmode == UpdateMode) {
        if(!m_mapper->submit()) {
            QMessageBox::warning(this,
                    tr("Update items"),
                    tr("Could not save item."),
                    QMessageBox::Ok);

            return;
        }
    } else if(mmode == InsertMode) {
        bool insert = m_model->setRow(0, prop);

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
    if(mmode == UpdateMode) {
        m_mapper->revert();
    }

    QDialog::reject();
}
