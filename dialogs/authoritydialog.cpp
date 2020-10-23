#include "authoritydialog.h"
#include "ui_authoritydialog.h"

#include <QMessageBox>
#include <QPushButton>

AuthorityDialog::AuthorityDialog(QVariant id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorityDialog)
{
    ui->setupUi(this);
    m_model = new QSqlTableModel;
    m_model->setTable("authority");
    m_model->setFilter("id=" + id.toString());
    m_model->select();

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_mapper->setModel(m_model);
    m_mapper->addMapping(ui->lE_name, 1);
    m_mapper->addMapping(ui->pTE_geo, 2);
    m_mapper->toFirst();

    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
        m_mapper->submit();
    });
    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &AuthorityDialog::reject);
}

AuthorityDialog::~AuthorityDialog()
{
    delete ui;
    delete m_model;
    delete m_mapper;
}

void AuthorityDialog::reject()
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
