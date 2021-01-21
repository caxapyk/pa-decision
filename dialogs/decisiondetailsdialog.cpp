#include "decisiondetailsdialog.h"
#include "ui_decisiondetailsdialog.h"

#include "application.h"
#include "dialogs/doctypedialog.h"
#include "dialogs/recorddialog.h"
#include "dialogs/protocoldialog.h"

#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QSqlError>

DecisionDetailsDialog::DecisionDetailsDialog(QVariant id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecisionDetailsDialog)
{
    ui->setupUi(this);
    restoreDialogState();

    m_model = new QSqlRelationalTableModel;
    m_model->setTable("pad_decision");
    m_model->setRelation(1, QSqlRelation("pad_record", "id", "number"));
    m_model->setRelation(2, QSqlRelation("pad_authority", "id", "name"));
    m_model->setRelation(3, QSqlRelation("pad_doctype", "id", "name"));
    m_model->setRelation(4, QSqlRelation("pad_protocol", "id", "number"));

    initialize();

    if(id.isValid()) {
        m_model->setFilter("pad_decision.id=" + id.toString());
        m_model->select();

        qDebug() << "row count" << m_model->rowCount();

        m_mapper = new QDataWidgetMapper;
        m_mapper->setItemDelegate(new QSqlRelationalDelegate);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        m_mapper->setModel(m_model);

        m_mapper->addMapping(ui->cB_record, 1);
        m_mapper->addMapping(ui->cB_authority, 2);
        m_mapper->addMapping(ui->cB_doctype, 3);
        m_mapper->addMapping(ui->cB_protocol, 4);
        m_mapper->addMapping(ui->dE_date, 5);
        m_mapper->addMapping(ui->lE_number, 6);
        m_mapper->addMapping(ui->lE_title, 7);
        //m_mapper->addMapping(ui->cB_access, 8);
        m_mapper->addMapping(ui->tE_content, 9);
        m_mapper->addMapping(ui->lE_comment, 10);

        m_mapper->toFirst();

        qDebug() << m_model->lastError().text();
    }

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &DecisionDetailsDialog::reject);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &DecisionDetailsDialog::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &DecisionDetailsDialog::accept);
}

DecisionDetailsDialog::~DecisionDetailsDialog()
{
    saveDialogState();

    delete ui;

    /*delete m_authorityModel;
    delete m_doctypeModel;
    delete m_protocolModel;
    delete m_recordModel;

    delete m_mapper;*/
}

void DecisionDetailsDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DecisionDetailsDialog/geometry").toByteArray());
}

void DecisionDetailsDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DecisionDetailsDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}

void DecisionDetailsDialog::initialize()
{
    /* Document type */
    m_doctypeModel = new DocumentTypeModel;
    m_doctypeModel->select();

    ui->cB_doctype->setModel(m_doctypeModel);
    ui->cB_doctype->setModelColumn(1);

    connect(ui->pB_doctype, &QPushButton::clicked, this, &DecisionDetailsDialog::openDoctypeDialog);

    /* Record */
    m_recordModel = new RecordFlatModel;
    connect(ui->pB_record, &QPushButton::clicked, this, &DecisionDetailsDialog::openRecordDialog);

    /* Protocol */
    m_protocolModel = new ProtocolFlatModel;
    connect(ui->gB_protocol, &QGroupBox::toggled, this, &DecisionDetailsDialog::protocolStateChanged);
    connect(ui->pB_protocol, &QPushButton::clicked, this,  &DecisionDetailsDialog::openProtocolDialog);

    /* Authority */
    m_authorityModel = new AuthorityFlatModel;
    m_authorityModel->select();

    connect(ui->cB_authority, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DecisionDetailsDialog::authorityChanged);
    ui->cB_authority->setModel(m_authorityModel);

    /* Access */
    connect(ui->cB_access, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DecisionDetailsDialog::accessChanged);
}

void DecisionDetailsDialog::authorityChanged(int index)
{
    QVariant authorityId = m_authorityModel->index(index, 2).data();
    if(!authorityId.isValid()) {
        return;
    }

    // load protocol
    if(ui->gB_protocol->isChecked()) {
        m_protocolModel->setAuthorityId(authorityId.toInt());
        m_protocolModel->select();
        ui->cB_protocol->setModel(m_protocolModel);
        ui->cB_protocol->setModelColumn(1);
    }
    // load record
    m_recordModel->setAuthorityId(authorityId.toInt());
    m_recordModel->select();
    ui->cB_record->setModel(m_recordModel);
    ui->cB_record->setModelColumn(1);
}

void DecisionDetailsDialog::protocolStateChanged(bool on)
{
    QVariant authorityId = m_authorityModel->index(ui->cB_authority->currentIndex(), 2).data();
    if(!authorityId.isValid()) {
        return;
    }

    if(on) {
        m_protocolModel->setAuthorityId(authorityId.toInt());
        m_protocolModel->select();
        ui->cB_protocol->setModel(m_protocolModel);
        ui->cB_protocol->setModelColumn(1);

        ui->cB_protocol->setEnabled(true);
    } else {
        m_protocolModel->clear();
        ui->cB_protocol->setModel(m_protocolModel);
    }
}

void DecisionDetailsDialog::accessChanged(int index)
{
    ui->cB_access->setStyleSheet(QString("background-color:%1;").arg(index ? "green" : "red"));
}

int DecisionDetailsDialog::exec()
{
    if(id().isValid()) {
        /*m_mapper = new QDataWidgetMapper;
        m_mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
        m_mapper->setModel(m_model);

        m_mapper->addMapping(ui->cB_authority, 2);
        m_mapper->addMapping(ui->lE_title, 7);

        m_mapper->toFirst();*/


    }
    /*if(setChosenId(ui->cB_authority, model()->authorityId(), 2)) {
        ui->cB_authority->setDisabled(true);
    }

    if(currentIndex().isValid()) {
        setWindowTitle(tr("Edit decision"));

        m_id = currentIndex().siblingAtColumn(0).data();

        m_mapper = new QDataWidgetMapper;
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        m_mapper->setModel(model());

        m_mapper->addMapping(ui->cB_record, 5);
        m_mapper->addMapping(ui->cB_doctype, 1);
        m_mapper->addMapping(ui->dE_date, 2);
        m_mapper->addMapping(ui->lE_number, 3);
        m_mapper->addMapping(ui->lE_title, 4);
        m_mapper->addMapping(ui->lE_comment, 7);
        m_mapper->addMapping(ui->tE_content, 8);

        m_mapper->setCurrentIndex(currentIndex().row());

        if(!model()->index(currentIndex().row(), 6).data().toString().isEmpty()) {
            m_protocolModel->select();

            ui->cB_protocol->setModel(m_protocolModel);
            ui->cB_protocol->setModelColumn(1);

            ui->gB_protocol->setChecked(true);
            ui->cB_protocol->setEnabled(true);

            m_mapper->addMapping(ui->cB_protocol, 6);
        }

        ui->cB_access->setCurrentIndex(currentIndex().siblingAtColumn(9).data().toInt());

        m_mapper->setCurrentIndex(currentIndex().row());
    } else {
        setWindowTitle(tr("New decision"));
        ui->dE_date->setDate(QDate::currentDate());
    }

    connect(ui->gB_protocol, &QGroupBox::toggled, this, &DecisionDetailsDialog::protocolStateChanged);
*/
    return QDialog::exec();
}

void DecisionDetailsDialog::openDoctypeDialog()
{
    DoctypeDialog dialog;
    openExternalDialog(ui->cB_doctype, &dialog);
}

void DecisionDetailsDialog::openProtocolDialog()
{
    ProtocolDialog dialog;
    //dialog.setAuthorityId(model()->authorityId());

    openExternalDialog(ui->cB_protocol, &dialog);
}

void DecisionDetailsDialog::openRecordDialog()
{
    RecordDialog dialog;
    //dialog.setAuthorityId(model()->authorityId());

    openExternalDialog(ui->cB_record, &dialog);
}


void DecisionDetailsDialog::openExternalDialog(QComboBox *cb, TreeDialog *dialog)
{
    SqlBaseModel *model = dynamic_cast<SqlBaseModel*>(cb->model());

    if(model) {
        dialog->setChoiceMode();
        int res = dialog->exec();

        model->select();

        if(res == RecordDialog::Accepted) {
            setChosenId(cb, dialog->currentChoice());
        }
    }
}

bool DecisionDetailsDialog::setChosenId(QComboBox *cb, int id, int column)
{
    QAbstractItemModel *model = cb->model();
    for (int i = 0; i < model->rowCount(); ++i) {
        if(model->index(i, column).data().toInt() == id) {
            cb->setCurrentIndex(i);
            return true;
        }
    }

    return false;
}

bool DecisionDetailsDialog::validate()
{
    bool v = !ui->lE_number->text().isEmpty() && !ui->lE_title->text().isEmpty()
            && ui->cB_authority->currentIndex() >= 0 && ui->cB_doctype->currentIndex() >=0;

    return v;
}

void DecisionDetailsDialog::save()
{
    if(!validate()) {
        QMessageBox::critical(this,
                              tr("New decision"),
                              tr("Could save decision. Fields are incorrect."),
                              QMessageBox::Ok);

        return;
    }

    if(m_mapper) {
        m_mapper->submit();
        m_model->submitAll();
        qDebug() << "all sumbited"<< m_model->lastError().text();
    }

    /*DecisionModel *m = dynamic_cast<DecisionModel*>(model());
    if(m) {
        qDebug() << id();
        bool saved = m->save(
                    m_recordModel->index(ui->cB_record->currentIndex(), 0).data(),
                    m_authorityModel->index(ui->cB_authority->currentIndex(), 2).data(),
                    m_doctypeModel->index(ui->cB_doctype->currentIndex(), 0).data(),
                    m_protocolModel->index(ui->cB_protocol->currentIndex(), 0).data(),
                    ui->dE_date->date(),
                    ui->lE_number->text(),
                    ui->lE_title->text(),
                    ui->cB_access->currentIndex(),
                    ui->tE_content->toHtml(),
                    ui->lE_comment->text(),
                    id().isValid() ? id() : QVariant()
                    );
        if(saved) {
            if(!id().isValid()) {
                m_id = m->lastInsertId();
            }
            setWindowTitle(tr("Edit decision"));
            ui->buttonBox->button(QDialogButtonBox::Discard)->setDisabled(true);
        } else {
            QMessageBox::critical(this,
                                  tr("Save decision"),
                                  tr("Could not save decision"),
                                  QMessageBox::Ok);
        }
    }*/
}

void DecisionDetailsDialog::accept()
{
    save();
    QDialog::accept();
}

void DecisionDetailsDialog::reject()
{
    int res = QMessageBox::critical(this,
                                    tr("Close decision"),
                                    tr("Are you shure that you want to dicard all changes?"),
                                    QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        QDialog::reject();
    }
}
