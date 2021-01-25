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

    initialize();
    restoreDialogState();

    if(id.isValid())
        setValues();

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &DecisionFormDialog::reject);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &DecisionFormDialog::accept);
}

DecisionFormDialog::~DecisionFormDialog()
{
    saveDialogState();

    delete ui;
    delete m_subjectsTable;
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
    m_subjectsTable = new Table;
    ui->tab_subject->layout()->addWidget(m_subjectsTable);

    ui->dE_date->setDate(QDate::currentDate());
    ui->lE_protcolPage->setValidator(new QRegExpValidator(QRegExp("\\d+")));

    connect(ui->cB_fund, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::updateInventory);
    connect(ui->cB_inventory, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::updateRecord);
    connect(ui->cB_record, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::updateProtocol);
    connect(ui->cB_protocol, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=](int index) {
             ui->lE_protcolPage->setEnabled(index >= 0);
    });

    connect(ui->gB_protocol, &QGroupBox::clicked, this, &DecisionFormDialog::useProtocolStateChanged);

    connect(ui->pB_fund, &QPushButton::clicked, this, &DecisionFormDialog::chooseFund);
    connect(ui->pB_record, &QPushButton::clicked, this, &DecisionFormDialog::chooseRecord);
    connect(ui->pB_inventory, &QPushButton::clicked, this, &DecisionFormDialog::chooseInventory);
    connect(ui->pB_protocol, &QPushButton::clicked, this, &DecisionFormDialog::chooseProtocol);

    connect(ui->cB_access, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DecisionFormDialog::accessStateChanged);

    updateAuthority();
    updateDoctype();
    updateFund();
}

void DecisionFormDialog::setValues()
{
    QSqlQuery query("select * from pad_decision where id=" + m_id.toString());

    query.exec();
    query.first();

    if(query.isActive() && query.size() > 0) {
        ui->cB_doctype->setCurrentIndex(m_doctypeIds.indexOf(query.value("doctype_id")));
        ui->lE_number->setText(query.value("number").toString());
        ui->dE_date->setDate(query.value("date").toDate());
        ui->lE_pages->setText(query.value("pages").toString());
        ui->pTE_title->setPlainText(query.value("name").toString());
        ui->cB_fund->setCurrentIndex(m_fundIds.indexOf(query.value("fund_id")));
        ui->cB_inventory->setCurrentIndex(m_inventoryIds.indexOf(query.value("inventory_id")));
        ui->cB_record->setCurrentIndex(m_recordIds.indexOf(query.value("record_id")));

        if(!query.value("protocol_id").isNull()) {
            ui->cB_protocol->setCurrentIndex(m_protocolIds.indexOf(query.value("protocol_id")));
            if (!query.value("protocol_page").isNull())
                ui->lE_protcolPage->setText(query.value("protocol_page").toString());
        } else {
            ui->gB_protocol->setChecked(false);
        }

        ui->cB_annexes->setChecked(query.value("annexes").toBool());
        ui->pTE_content->setPlainText(query.value("content").toString());
        ui->pTE_comment->setPlainText(query.value("comment").toString());
        ui->cB_access->setCurrentIndex(query.value("access").toInt());
    }
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
    ui->cB_inventory->setEnabled(fundIndex >= 0);
    ui->pB_inventory->setEnabled(fundIndex >= 0);
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

    ui->lE_protcolPage->clear();
}

void DecisionFormDialog::chooseFund()
{
    RecordDialog dialog(m_authorityId);
    dialog.setChoiceMode();
    dialog.setChoiceLevel(RecordModel::FundLevel);

    int res = dialog.exec();

    updateFund();
    if(res == RecordDialog::Accepted) {
        ui->cB_fund->setCurrentIndex(m_fundIds.indexOf(dialog.currentChoice()));
    }
}

void DecisionFormDialog::chooseInventory()
{
    RecordDialog dialog(m_authorityId, m_fundIds.at(ui->cB_fund->currentIndex()));
    dialog.setChoiceMode();
    dialog.setChoiceLevel(RecordModel::InventoryLevel);

    int res = dialog.exec();

    updateInventory(ui->cB_fund->currentIndex());
    if(res == RecordDialog::Accepted) {
        ui->cB_inventory->setCurrentIndex(m_inventoryIds.indexOf(dialog.currentChoice()));
    }
}

void DecisionFormDialog::chooseRecord()
{
    RecordDialog dialog(m_authorityId, m_fundIds.at(ui->cB_fund->currentIndex()), m_inventoryIds.at(ui->cB_inventory->currentIndex()));
    dialog.setChoiceMode();
    dialog.setChoiceLevel(RecordModel::RecordLevel);

    int res = dialog.exec();

    updateRecord(ui->cB_inventory->currentIndex());
    if(res == RecordDialog::Accepted) {
        ui->cB_record->setCurrentIndex(m_recordIds.indexOf(dialog.currentChoice()));
    }
}

void DecisionFormDialog::chooseProtocol()
{
    ProtocolDialog dialog(m_recordIds.at(ui->cB_record->currentIndex()));
    dialog.setChoiceMode();

    int res = dialog.exec();

    updateProtocol(ui->cB_record->currentIndex());
    if(res == ProtocolDialog::Accepted) {
        ui->cB_protocol->setCurrentIndex(m_protocolIds.indexOf(dialog.currentChoice()));
        ui->lE_protcolPage->setEnabled(true);
    } else {
        ui->lE_protcolPage->setDisabled(true);
    }
}

void DecisionFormDialog::useProtocolStateChanged(bool checked)
{
    if(!checked) {
        ui->cB_protocol->setCurrentIndex(-1);
        ui->lE_protcolPage->clear();
    } else {
        ui->lE_protcolPage->setDisabled(true);
    }
}

void DecisionFormDialog::accessStateChanged(int index)
{
    ui->cB_access->setStyleSheet(QString("background-color:%1;").arg(index ? "green" : "red"));
}

bool DecisionFormDialog::validate()
{
    if(ui->lE_number->text().isEmpty()) {
        QMessageBox::critical(this, tr("Document form"), tr("Please enter number."), QMessageBox::Ok);
        return false;
    } else if(ui->pTE_title->toPlainText().isEmpty()) {
        QMessageBox::critical(this, tr("Document form"), tr("Please enter title."), QMessageBox::Ok);
        return false;
    } else if(ui->cB_fund->currentIndex() < 0) {
        QMessageBox::critical(this, tr("Document form"), tr("Please choose fund."), QMessageBox::Ok);
        return false;
    } else if(ui->cB_inventory->currentIndex() < 0) {
        QMessageBox::critical(this, tr("Document form"), tr("Please choose inventory."), QMessageBox::Ok);
        return false;
    } else if(ui->cB_record->currentIndex() < 0) {
        QMessageBox::critical(this, tr("Document form"), tr("Please choose record."), QMessageBox::Ok);
        return false;
    } else if(ui->gB_protocol->isChecked() && ui->cB_protocol->currentIndex() < 0) {
        QMessageBox::critical(this, tr("Document form"), tr("Please choose protocol or diselect."), QMessageBox::Ok);
        return false;
    } else if(ui->pTE_title->toPlainText().length() > 255) {
        QMessageBox::critical(this, tr("Document form"), tr("Title is too long."), QMessageBox::Ok);
        return false;
    } else if(ui->pTE_comment->toPlainText().length() > 255) {
        QMessageBox::critical(this, tr("Document form"), tr("Comment is too long."), QMessageBox::Ok);
        return false;
    }

    return true;
}

QVariant DecisionFormDialog::getAuthority() const
{
    return m_authorityId;
}

QVariant DecisionFormDialog::getDoctype() const
{
    return m_doctypeIds.at(ui->cB_doctype->currentIndex());
}

QVariant DecisionFormDialog::getNumber() const
{
    return QVariant(ui->lE_number->text());
}

QVariant DecisionFormDialog::getDate() const
{
    return QVariant(ui->dE_date->date());
}

QVariant DecisionFormDialog::getTitle() const
{
    return QVariant(ui->pTE_title->toPlainText());
}

QVariant DecisionFormDialog::getPages() const
{
    QString v = ui->lE_pages->text();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DecisionFormDialog::getAnnexes() const
{
    return QVariant(ui->cB_annexes->isChecked());
}

QVariant DecisionFormDialog::getFund() const
{
    return QVariant(m_fundIds.at(ui->cB_fund->currentIndex()));
}

QVariant DecisionFormDialog::getInventory() const
{
    return QVariant(m_inventoryIds.at(ui->cB_inventory->currentIndex()));
}

QVariant DecisionFormDialog::getRecord() const
{
    return QVariant(m_recordIds.at(ui->cB_record->currentIndex()));
}

QVariant DecisionFormDialog::getProtocol() const
{
    int v = ui->cB_protocol->currentIndex();
    return  v > 0 ? QVariant(m_protocolIds.at(v)) : QVariant();
}

QVariant DecisionFormDialog::getProtocolPage() const
{
    QString v = ui->lE_protcolPage->text();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DecisionFormDialog::getContent() const
{
    QString v = ui->pTE_content->toPlainText();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DecisionFormDialog::getComment() const
{
    QString v = ui->pTE_comment->toPlainText();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DecisionFormDialog::getAccess() const
{
    return QVariant(ui->cB_access->currentIndex());
}

void DecisionFormDialog::accept()
{
    if(!validate())
        return;

    QDialog::accept();
}

void DecisionFormDialog::reject()
{
    int res = QMessageBox::critical(this,
                                    tr("Document form"),
                                    tr("Are you shure that you want to dicard all changes?"),
                                    QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        QDialog::reject();
    }
}
