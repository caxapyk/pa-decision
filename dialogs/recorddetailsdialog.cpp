#include "recorddetailsdialog.h"
#include "ui_recorddetailsdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlTableModel>

RecordDetailsDialog::RecordDetailsDialog(const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDetailsDialog)
{
    m_id = id;
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &
            RecordDetailsDialog::reject);
}

RecordDetailsDialog::~RecordDetailsDialog()
{
    delete ui;
    delete m_mapper;
}

int RecordDetailsDialog::exec()
{
    QSqlTableModel model;
    model.setTable("pad_record");
    model.setFilter("id=" + m_id.toString());
    model.select();

    if (model.rowCount() > 0) {
        setWindowTitle(tr("Edit record"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(&model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 2);
        m_mapper->addMapping(ui->lE_pages, 3);
        m_mapper->addMapping(ui->lE_title, 4);
        m_mapper->addMapping(ui->lE_comment, 5);
        m_mapper->setCurrentIndex(0);

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
            if(m_mapper->submit()) {
                accept();
            } else {
                QMessageBox::critical(this,
                                      tr("Record"),
                                      tr("Could not save record"),
                                      QMessageBox::Ok);
            }
        });

    }

    return QDialog::exec();
}

QString RecordDetailsDialog::getNumber() const
{
    return ui->lE_number->text();
}

QString RecordDetailsDialog::getName() const
{
    return ui->lE_title->text();
}

void RecordDetailsDialog::reject()
{
    bool commit = ui->lE_title->isModified() || ui->lE_number->isModified() || ui->lE_pages->isModified() || ui->lE_comment->isModified();
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
