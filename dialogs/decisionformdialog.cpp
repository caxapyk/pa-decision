#include "decisionformdialog.h"
#include "ui_decisionformdialog.h"

#include "application.h"
#include "dialogs/doctypedialog.h"
#include "dialogs/recorddialog.h"
#include "dialogs/protocoldialog.h"

#include <QDebug>
#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringListModel>
#include <QComboBox>

DecisionFormDialog::DecisionFormDialog(const QVariant &authorityId, const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecisionFormDialog),
    m_authorityId(authorityId),
    m_id(id)
{
    ui->setupUi(this);
    restoreDialogState();

    initialize();

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &DecisionFormDialog::reject);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &DecisionFormDialog::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &DecisionFormDialog::accept);
}

DecisionFormDialog::~DecisionFormDialog()
{
    saveDialogState();

    delete ui;
}

void DecisionFormDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DecisionFormDialog/geometry").toByteArray());
}

void DecisionFormDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DecisionFormDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}

void DecisionFormDialog::initialize()
{
    ui->dE_date->setDate(QDate::currentDate());

    updateAuthority();
    updateDoctype();
    updateFund();

    connect(ui->cB_fund, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::updateInventory);
    connect(ui->cB_inventory, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::updateRecord);
    connect(ui->cB_record, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::updateProtocol);

    connect(ui->gB_protocol, &QGroupBox::clicked, [=](bool checked) {
        if(!checked) {
            ui->cB_protocol->setCurrentIndex(-1);
            ui->lE_protcolPage->clear();
        }
    });

    connect(ui->pB_fund, &QPushButton::clicked, this, &DecisionFormDialog::chooseFund);
    connect(ui->pB_record, &QPushButton::clicked, this, &DecisionFormDialog::chooseRecord);
    connect(ui->pB_inventory, &QPushButton::clicked, this, &DecisionFormDialog::chooseInventory);
    connect(ui->pB_protocol, &QPushButton::clicked, this, &DecisionFormDialog::chooseProtocol);
}

void DecisionFormDialog::updateAuthority()
{
    QSqlQuery query;

    query.prepare("select id, name from pad_authority where id=?");
    query.bindValue(0, m_authorityId);
    query.exec();

    ui->cB_authority->clear();

    while(query.next()) {
        m_authorityIds.append(query.value(0));
        ui->cB_authority->addItem(query.value(1).toString());
    }
}

void DecisionFormDialog::updateDoctype()
{
    QSqlQuery query;
    query.prepare("select id, name from pad_doctype");
    query.exec();

    ui->cB_doctype->clear();

    if(!m_doctypeIds.isEmpty())
        m_doctypeIds.clear();

    while(query.next()) {
        QVariant val = query.value(1);
        m_doctypeIds.append(query.value(0));
        ui->cB_doctype->addItem(val.toString());
    }
}

void DecisionFormDialog::updateFund()
{
    QSqlQuery query;
    query.prepare("select id, number from pad_fund where authority_id=?");
    query.bindValue(0, m_authorityId);
    query.exec();

    ui->cB_fund->clear();

    if(!m_fundIds.isEmpty())
        m_fundIds.clear();

    while(query.next()) {
        QVariant val = query.value(1);
        m_fundIds.append(query.value(0));
        ui->cB_fund->addItem(val.toString());
    }

    ui->cB_fund->setCurrentIndex(-1);
 }

void DecisionFormDialog::updateInventory(int fundIndex)
{
    QSqlQuery query;
    query.prepare("select id, number from pad_inventory where fund_id=?");
    query.bindValue(0, m_fundIds.value(fundIndex));
    query.exec();

    ui->cB_inventory->clear();

    if(!m_inventoryIds.isEmpty())
        m_inventoryIds.clear();

    while(query.next()) {
        QVariant val = query.value(1);
        m_inventoryIds.append(query.value(0));
        ui->cB_inventory->addItem(val.toString());
    }

    ui->cB_inventory->setCurrentIndex(-1);
    ui->cB_inventory->setEnabled(fundIndex > -1);
    ui->pB_inventory->setEnabled(fundIndex > -1);
}

void DecisionFormDialog::updateRecord(int inventoryIndex)
{
    QSqlQuery query;
    query.prepare("select id, number from pad_record where inventory_id=?");
    query.bindValue(0, m_inventoryIds.value(inventoryIndex));
    query.exec();

    ui->cB_record->clear();

    if(!m_recordIds.isEmpty())
        m_recordIds.clear();

    while(query.next()) {
        QVariant val = query.value(1);
        m_recordIds.append(query.value(0));
        ui->cB_record->addItem(val.toString());
    }

    ui->cB_record->setCurrentIndex(-1);
    ui->cB_record->setEnabled(inventoryIndex > -1);
    ui->pB_record->setEnabled(inventoryIndex > -1);
}

void DecisionFormDialog::updateProtocol(int recordIndex)
{
    QSqlQuery query;
    query.prepare("select id, CONCAT('№', number, ', ', date) from pad_protocol where record_id=?");
    query.bindValue(0, m_recordIds.value(recordIndex));
    query.exec();

    ui->cB_protocol->clear();

    if(!m_protocolIds.isEmpty())
        m_protocolIds.clear();

    while(query.next()) {
        QVariant val = query.value(1);
        m_protocolIds.append(query.value(0));
        ui->cB_protocol->addItem(val.toString());
    }

    ui->cB_protocol->setCurrentIndex(-1);
    ui->cB_protocol->setEnabled(recordIndex > -1);
    ui->pB_protocol->setEnabled(recordIndex > -1);
}

void DecisionFormDialog::chooseFund()
{
    RecordDialog dialog;
    dialog.setChoiceMode();
    dialog.setChoiceLevel(RecordModel::FundLevel);
    dialog.setAuthorityId(m_authorityId);

    int res = dialog.exec();

    if(res == RecordDialog::Accepted) {
        if(!m_fundIds.contains(dialog.currentChoice())) {
            updateFund();
        }
        ui->cB_fund->setCurrentIndex(m_fundIds.indexOf(dialog.currentChoice()));
    }
}

void DecisionFormDialog::chooseInventory()
{
    RecordDialog dialog;
    dialog.setChoiceMode();
    dialog.setChoiceLevel(RecordModel::InventoryLevel);
    dialog.setAuthorityId(m_authorityId);

    int res = dialog.exec();

    if(res == RecordDialog::Accepted) {
        if(!m_inventoryIds.contains(dialog.currentChoice())) {
            updateInventory(ui->cB_fund->currentIndex());
        }
        ui->cB_inventory->setCurrentIndex(m_inventoryIds.indexOf(dialog.currentChoice()));
    }
}

void DecisionFormDialog::chooseRecord()
{
    RecordDialog dialog;
    dialog.setChoiceMode();
    dialog.setChoiceLevel(RecordModel::RecordLevel);
    dialog.setAuthorityId(m_authorityId);

    int res = dialog.exec();

    if(res == RecordDialog::Accepted) {
        if(!m_recordIds.contains(dialog.currentChoice())) {
            updateRecord(ui->cB_inventory->currentIndex());
        }
        ui->cB_record->setCurrentIndex(m_recordIds.indexOf(dialog.currentChoice()));
    }
}

void DecisionFormDialog::chooseProtocol()
{
    ProtocolDialog dialog(m_protocolIds.at(ui->cB_record->currentIndex()));
    dialog.setChoiceMode();

    int res = dialog.exec();

    if(res == ProtocolDialog::Accepted) {
        if(!m_protocolIds.contains(dialog.currentChoice())) {
            updateProtocol(ui->cB_protocol->currentIndex());
        }
        ui->cB_protocol->setCurrentIndex(m_protocolIds.indexOf(dialog.currentChoice()));
    }
}

int DecisionFormDialog::exec()
{
    return QDialog::exec();
}

bool DecisionFormDialog::validate()
{
    /*bool v = !ui->lE_number->text().isEmpty() && !ui->lE_title->text().isEmpty()
            && ui->cB_authority->currentIndex() >= 0 && ui->cB_doctype->currentIndex() >=0;*/

    return false;
}

void DecisionFormDialog::save()
{
    /*if(!validate()) {
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

void DecisionFormDialog::accept()
{
    /*save();*/
    QDialog::accept();
}

void DecisionFormDialog::reject()
{
    /*int res = QMessageBox::critical(this,
                                    tr("Close decision"),
                                    tr("Are you shure that you want to dicard all changes?"),
                                    QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        QDialog::reject();
    }*/

    QDialog::reject();
}
