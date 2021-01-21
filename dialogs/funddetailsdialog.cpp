#include "funddetailsdialog.h"
#include "ui_funddetailsdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlTableModel>

FundDetailsDialog::FundDetailsDialog(const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FundDetailsDialog)
{
    m_id = id;
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &FundDetailsDialog::reject);
}

FundDetailsDialog::~FundDetailsDialog()
{
    delete ui;
}

int FundDetailsDialog::exec()
{
    QSqlTableModel model;
    model.setTable("pad_fund");
    model.setFilter("id=" + m_id.toString());
    model.select();

    QModelIndex index = model.index(0, 0);

    if (index.isValid()) {
        setWindowTitle(tr("Edit fund"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(&model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 2);
        m_mapper->addMapping(ui->lE_title, 3);
        m_mapper->addMapping(ui->lE_comment, 4);
        m_mapper->setCurrentIndex(index.row());

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
            if(m_mapper->submit()) {
                accept();
            } else {
                QMessageBox::critical(this,
                                      tr("Fund"),
                                      tr("Could not save fund"),
                                      QMessageBox::Ok);
            }
        });

    }

    return QDialog::exec();
}

QString FundDetailsDialog::getNumber() const
{
    return ui->lE_number->text();
}

QString FundDetailsDialog::getName() const
{
    return ui->lE_title->text();
}

void FundDetailsDialog::reject()
{
    bool commit = ui->lE_title->isModified() || ui->lE_number->isModified() || ui->lE_comment->isModified();
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
