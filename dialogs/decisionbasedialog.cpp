#include "decisionbasedialog.h"
#include "ui_decisionbasedialog.h"

#include "application.h"
#include "dialogs/decisioneditdialog.h"
#include "dialogs/doctypedialog.h"
#include "dialogs/recorddialog.h"
#include "dialogs/protocoldialog.h"

#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QSqlRelationalDelegate>

DecisionBaseDialog::DecisionBaseDialog(QWidget *parent) :
    DetailsDialog(parent),
    ui(new Ui::DecisionBaseDialog)
{
    ui->setupUi(this);
    restoreDialogState();
}

DecisionBaseDialog::~DecisionBaseDialog()
{
    saveDialogState();

    delete ui;

    delete m_authorityModel;
    delete m_doctypeModel;
    delete m_protocolModel;
    delete m_recordModel;

    delete m_mapper;
}

void DecisionBaseDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DecisionBaseDialog/geometry").toByteArray());
}

void DecisionBaseDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DecisionBaseDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}

int DecisionBaseDialog::exec()
{
    m_authorityModel = new AuthorityFlatModel;
    m_authorityModel->select();
    ui->cB_authority->setModel(m_authorityModel);

    if(setChosenId(ui->cB_authority, model()->authorityId(), 2)) {
        ui->cB_authority->setDisabled(true);
    }

    m_recordModel = new RecordFlatModel;
    m_recordModel->setAuthorityId(model()->authorityId());
    m_recordModel->select();
    ui->cB_record->setModel(m_recordModel);
    ui->cB_record->setModelColumn(1);

    m_protocolModel = new ProtocolFlatModel;
    m_protocolModel->setAuthorityId(model()->authorityId());

    m_doctypeModel = new DocumentTypeModel;
    m_doctypeModel->select();
    ui->cB_doctype->setModel(m_doctypeModel);
    ui->cB_doctype->setModelColumn(1);

    connect(ui->pB_doctype, &QPushButton::clicked, this, &DecisionBaseDialog::openDoctypeDialog);
    connect(ui->pB_protocol, &QPushButton::clicked, this,  &DecisionBaseDialog::openProtocolDialog);
    connect(ui->pB_record, &QPushButton::clicked, this, &DecisionBaseDialog::openRecordDialog);

    connect(ui->cB_access, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DecisionBaseDialog::accessChanged);

    if(currentIndex().isValid()) {
        setWindowTitle(tr("Edit decision"));

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
        m_mapper->addMapping(ui->cB_access, 9);

        m_mapper->setCurrentIndex(currentIndex().row());

        if(model()->index(0, 6).data().isValid()) {
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

    connect(ui->gB_protocol, &QGroupBox::toggled, this, &DecisionBaseDialog::protocolStateChanged);

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &DecisionBaseDialog::reject);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &DecisionBaseDialog::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &DecisionBaseDialog::accept);

    return DetailsDialog::exec();
}

/*void DecisionBaseDialog::authorityChanged(int index)
{
    // load protocol
    if(ui->gB_protocol->isChecked()) {

        m_protocolModel->setAuthorityId(model()->authorityId());
        m_protocolModel->select();

        ui->cB_protocol->setModel(m_protocolModel);
        ui->cB_protocol->setModelColumn(1);
    }

    // load record
    m_recordModel->setAuthorityId(model()->authorityId());
    m_recordModel->select();
    ui->cB_record->setModel(m_recordModel);
    ui->cB_record->setModelColumn(1);
}*/

void DecisionBaseDialog::protocolStateChanged(bool on)
{
    if(on) {
        m_protocolModel->setAuthorityId(model()->authorityId());
        m_protocolModel->select();
        ui->cB_protocol->setModel(m_protocolModel);
        ui->cB_protocol->setModelColumn(1);

        ui->cB_protocol->setEnabled(true);
    } else {
        m_protocolModel->clear();
        ui->cB_protocol->setModel(m_protocolModel);
    }
}

void DecisionBaseDialog::accessChanged(int index)
{
    ui->cB_access->setStyleSheet(QString("background-color:%1;").arg(index ? "green" : "red"));
}

void DecisionBaseDialog::openDoctypeDialog()
{
    DoctypeDialog dialog;
    openExternalDialog(ui->cB_doctype, &dialog);
}

void DecisionBaseDialog::openProtocolDialog()
{
    ProtocolDialog dialog;
    dialog.setAuthorityId(model()->authorityId());

    openExternalDialog(ui->cB_protocol, &dialog);
}


void DecisionBaseDialog::openRecordDialog()
{
    RecordDialog dialog;
    dialog.setAuthorityId(model()->authorityId());

    openExternalDialog(ui->cB_record, &dialog);
}


void DecisionBaseDialog::openExternalDialog(QComboBox *cb, ReferenceDialog *dialog)
{
    ReferenceModel *model = dynamic_cast<ReferenceModel*>(cb->model());

    if(model) {
        dialog->setChoiceMode();
        int res = dialog->exec();

        model->select();

        if(res == RecordDialog::Accepted) {
            setChosenId(cb, dialog->currentChoice());
        }
    }
}

bool DecisionBaseDialog::setChosenId(QComboBox *cb, int id, int column)
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

bool DecisionBaseDialog::validate()
{
    bool v = !ui->lE_number->text().isEmpty() && !ui->lE_title->text().isEmpty()
            && ui->cB_authority->currentIndex() >= 0 && ui->cB_doctype->currentIndex() >=0;

    return v;
}

void DecisionBaseDialog::save()
{
    if(!validate()) {
        QMessageBox::critical(this,
                              tr("New protocol"),
                              tr("Could save decision. Fields are incorrect."),
                              QMessageBox::Ok);

        return;
    }

    DecisionReadModel *m = dynamic_cast<DecisionReadModel*>(model());
    if(m) {
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
                    currentIndex().isValid() ? currentIndex().siblingAtColumn(0).data() : QVariant()
                    );
        if(saved) {
            qDebug() << "saved" << m->lastInsertId();
            accept();
        } else {
            QMessageBox::critical(this,
                                  tr("Save decision"),
                                  tr("Could not save decision"),
                                  QMessageBox::Ok);
        }
    }
}

void DecisionBaseDialog::reject()
{
    int res = QMessageBox::critical(this,
                                    tr("Close decision"),
                                    tr("Are you shure that you want to dicard all changes?"),
                                    QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        QDialog::reject();
    }
}
