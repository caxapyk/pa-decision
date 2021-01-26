#include "documentformdialog.h"
#include "ui_documentformdialog.h"

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

DocumentFormDialog::DocumentFormDialog(const QVariant &authorityId, const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentFormDialog),
    m_authorityId(authorityId),
    m_id(id)
{
    ui->setupUi(this);

    m_subjectsTable = new QTableWidget;

    initialize();
    restoreDialogState();
}

DocumentFormDialog::~DocumentFormDialog()
{
    saveDialogState();

    delete ui;
    delete m_subjectsTable;
}

void DocumentFormDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DocumentFormDialog/geometry").toByteArray());
}

void DocumentFormDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DocumentFormDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}

void DocumentFormDialog::initialize()
{
    m_subjectsTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_subjectsTable->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    m_subjectsTable->setColumnCount(5);
    m_subjectsTable->setHorizontalHeaderLabels(m_subjectHeaderLabels);
    ui->tab_subject->layout()->addWidget(m_subjectsTable);

    ui->dE_date->setDate(QDate::currentDate());
    ui->lE_protcolPage->setValidator(new QRegExpValidator(QRegExp("\\d+")));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(ui->cB_fund, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DocumentFormDialog::updateInventory);
    connect(ui->cB_inventory, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DocumentFormDialog::updateRecord);
    connect(ui->cB_record, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DocumentFormDialog::updateProtocol);
    connect(ui->cB_protocol, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=](int index) {
             ui->lE_protcolPage->setEnabled(index >= 0);
    });

    connect(ui->gB_protocol, &QGroupBox::clicked, this, &DocumentFormDialog::useProtocolStateChanged);

    connect(ui->pB_fund, &QPushButton::clicked, this, &DocumentFormDialog::chooseFund);
    connect(ui->pB_record, &QPushButton::clicked, this, &DocumentFormDialog::chooseRecord);
    connect(ui->pB_inventory, &QPushButton::clicked, this, &DocumentFormDialog::chooseInventory);
    connect(ui->pB_protocol, &QPushButton::clicked, this, &DocumentFormDialog::chooseProtocol);

    connect(ui->cB_access, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DocumentFormDialog::accessStateChanged);

    //connect(m_subjectsTable, &Table::onInsert, this, &DocumentFormDialog::insertSubject);
    //connect(m_subjectsTable, &Table::onRemove, this, &DocumentFormDialog::removeSubject);

    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &DocumentFormDialog::reject);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &DocumentFormDialog::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &DocumentFormDialog::accept);

    updateAuthority();
    updateDoctype();
    updateFund();

    if(m_id.isValid())
        setValues();
}

void DocumentFormDialog::setValues()
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

void DocumentFormDialog::updateAuthority()
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

void DocumentFormDialog::updateDoctype()
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

void DocumentFormDialog::updateFund()
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

void DocumentFormDialog::updateInventory(int fundIndex)
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

void DocumentFormDialog::updateRecord(int inventoryIndex)
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

void DocumentFormDialog::updateProtocol(int recordIndex)
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

void DocumentFormDialog::chooseFund()
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

void DocumentFormDialog::chooseInventory()
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

void DocumentFormDialog::chooseRecord()
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

void DocumentFormDialog::chooseProtocol()
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

void DocumentFormDialog::insertSubject()
{
    int row = m_subjectsTable->rowCount();

    m_subjectsTable->setSortingEnabled(false);

    m_subjectsTable->insertRow(row);
    m_subjectsTable->selectRow(row);

    m_subjectsTable->setSortingEnabled(true);
}

void DocumentFormDialog::removeSubject(const QModelIndex &index)
{
    m_subjectsTable->removeRow(index.row());
}

void DocumentFormDialog::useProtocolStateChanged(bool checked)
{
    if(!checked) {
        ui->cB_protocol->setCurrentIndex(-1);
        ui->lE_protcolPage->clear();
    } else {
        ui->lE_protcolPage->setDisabled(true);
    }
}

void DocumentFormDialog::accessStateChanged(int index)
{
    ui->cB_access->setStyleSheet(QString("background-color:%1;").arg(index ? "green" : "red"));
}

bool DocumentFormDialog::validate()
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

void DocumentFormDialog::save()
{
    if(!validate())
        return;

    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    if(saveForm() && saveSubjects()) {
        db.commit();
    } else {
        db.rollback();
        QMessageBox::warning(this,
                tr("Document form"),
                m_id.isValid() ? tr("Could not save data.") : tr("Could not create item."),
                QMessageBox::Ok);
    }
}

bool DocumentFormDialog::saveForm()
{
    QSqlQuery query;

    if(m_id.isValid()) {
        query.prepare("update pad_decision SET authority_id=:authority_id, fund_id=:fund_id, inventory_id=:inventory_id, record_id=:record_id, doctype_id=:doctype_id, protocol_id=:protocol_id, number=:number, date=:date, name=:name, pages=:pages, protocol_page=:protocol_page, annexes=:annexes, content=:content, comment=:comment, access=:access where id=:id");
        query.bindValue(":id", m_id);
    } else {
        query.prepare("insert into pad_decision values (:id, :authority_id, :fund_id, :inventory_id, :record_id, :doctype_id, :protocol_id, :number, :date, :name, :pages, :protocol_page, :annexes, :content, :comment, :access)");
    }

    query.bindValue(":authority_id", getAuthority());
    query.bindValue(":fund_id", getFund());
    query.bindValue(":inventory_id", getInventory());
    query.bindValue(":record_id", getRecord());
    query.bindValue(":doctype_id", getDoctype());
    query.bindValue(":protocol_id", getProtocol());
    query.bindValue(":number", getNumber());
    query.bindValue(":date", getDate());
    query.bindValue(":name", getTitle());
    query.bindValue(":pages", getPages());
    query.bindValue(":protocol_page", getProtocolPage());
    query.bindValue(":annexes", getAnnexes());
    query.bindValue(":content", getContent());
    query.bindValue(":comment", getComment());
    query.bindValue(":access", getAccess());

    query.exec();

    if(query.isActive()) {
        if(m_id.isNull())
            m_id = query.lastInsertId();

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool DocumentFormDialog::saveSubjects()
{
   /* QSqlQuery query;

    query.prepare("insert into pad_subjects values (:id, :authority_id, :fund_id, :inventory_id, :record_id, :doctype_id, :protocol_id, :number, :date, :name, :pages, :protocol_page, :annexes, :content, :comment, :access)");
    query.exec();

    if(query.isActive()) {
        if(m_id.isNull())
            m_id = query.lastInsertId();

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        qDebug() << query.lastError().text();
        return false;
    }*/

    return true;
}

QVariant DocumentFormDialog::getAuthority() const
{
    return m_authorityId;
}

QVariant DocumentFormDialog::getDoctype() const
{
    return m_doctypeIds.at(ui->cB_doctype->currentIndex());
}

QVariant DocumentFormDialog::getNumber() const
{
    return QVariant(ui->lE_number->text());
}

QVariant DocumentFormDialog::getDate() const
{
    return QVariant(ui->dE_date->date());
}

QVariant DocumentFormDialog::getTitle() const
{
    return QVariant(ui->pTE_title->toPlainText());
}

QVariant DocumentFormDialog::getPages() const
{
    QString v = ui->lE_pages->text();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DocumentFormDialog::getAnnexes() const
{
    return QVariant(ui->cB_annexes->isChecked());
}

QVariant DocumentFormDialog::getFund() const
{
    return QVariant(m_fundIds.at(ui->cB_fund->currentIndex()));
}

QVariant DocumentFormDialog::getInventory() const
{
    return QVariant(m_inventoryIds.at(ui->cB_inventory->currentIndex()));
}

QVariant DocumentFormDialog::getRecord() const
{
    return QVariant(m_recordIds.at(ui->cB_record->currentIndex()));
}

QVariant DocumentFormDialog::getProtocol() const
{
    int v = ui->cB_protocol->currentIndex();
    return  v > 0 ? QVariant(m_protocolIds.at(v)) : QVariant();
}

QVariant DocumentFormDialog::getProtocolPage() const
{
    QString v = ui->lE_protcolPage->text();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DocumentFormDialog::getContent() const
{
    QString v = ui->pTE_content->toPlainText();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DocumentFormDialog::getComment() const
{
    QString v = ui->pTE_comment->toPlainText();
    return v.length() > 0 ? QVariant(v) : QVariant();
}

QVariant DocumentFormDialog::getAccess() const
{
    return QVariant(ui->cB_access->currentIndex());
}

void DocumentFormDialog::accept()
{
    QDialog::accept();
}

void DocumentFormDialog::reject()
{
    int res = QMessageBox::critical(this,
                                    tr("Document form"),
                                    tr("Are you shure that you want to dicard all changes?"),
                                    QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        QDialog::reject();
    }
}
