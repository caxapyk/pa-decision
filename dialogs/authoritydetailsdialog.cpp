#include "authoritydetailsdialog.h"
#include "ui_authoritydetailsdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

AuthorityDetailsDialog::AuthorityDetailsDialog(QVariant id, QWidget *parent) :
    DetailsDialog(parent),
    ui(new Ui::AuthorityDetailsDialog)
{
    ui->setupUi(this);
    m_model = new QSqlTableModel;
    m_model->setTable("pad_authority");
    m_model->setFilter("id=" + id.toString());
    m_model->select();

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_mapper->setModel(m_model);
    m_mapper->addMapping(ui->lE_name, 1);
    m_mapper->addMapping(ui->lE_shortName, 2);
    m_mapper->addMapping(ui->pTE_geo, 3);
    m_mapper->addMapping(ui->lE_comment, 4);
    m_mapper->toFirst();

    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
        if(ui->lE_name->text().length() > 0 && m_mapper->submit()) {
            accept();
        } else {
            QMessageBox::warning(this,
                                 tr("Authority details"),
                                 tr("Could not save data."),
                                 QMessageBox::Ok);
        }
    });
    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &AuthorityDetailsDialog::reject);
}

AuthorityDetailsDialog::~AuthorityDetailsDialog()
{
    delete ui;
    delete m_model;
    delete m_mapper;
}

QString AuthorityDetailsDialog::getName() const
{
    return ui->lE_name->text();
}

void AuthorityDetailsDialog::reject()
{
    bool commit = ui->lE_name->isModified() || ui->pTE_geo->document()->isModified();
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
