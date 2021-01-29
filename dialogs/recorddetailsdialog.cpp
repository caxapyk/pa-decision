#include "recorddetailsdialog.h"
#include "ui_recorddetailsdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QRegExpValidator>

RecordDetailsDialog::RecordDetailsDialog(const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDetailsDialog)
{
    m_id = id;
    ui->setupUi(this);

     ui->lE_pages->setValidator(new QRegExpValidator(QRegExp("\\d+")));

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
    m_model.setTable("pad_record");
    m_model.setFilter("id=" + m_id.toString());
    m_model.select();

    if (m_model.rowCount() > 0) {
        setWindowTitle(tr("Record details"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(&m_model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 2);
        m_mapper->addMapping(ui->lE_pages, 3);
        m_mapper->addMapping(ui->lE_title, 4);
        m_mapper->addMapping(ui->lE_comment, 5);
        m_mapper->setCurrentIndex(0);

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
            if(!validate())
                return;

            if(m_mapper->submit()) {
                accept();
            } else {
                QMessageBox::critical(this,
                                      tr("Record details"),
                                      tr("Could not save data."),
                                      QMessageBox::Ok);
            }
        });

    }

    return QDialog::exec();
}

bool RecordDetailsDialog::validate()
{
    if(ui->lE_pages->text().isEmpty()) {
        m_mapper->removeMapping(ui->lE_pages);
        m_model.setData(m_model.index(0, 3), QVariant());
    }

    if(ui->lE_number->text().isEmpty() || ui->lE_title->text().isEmpty()) {
        QMessageBox::critical(this,
                              tr("Record details"),
                              tr("Fill all required fields (*)."),
                              QMessageBox::Ok);
        return false;
    }

    return true;
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
