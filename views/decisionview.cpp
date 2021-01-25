#include "decisionview.h"
#include "ui_decisionview.h"

#include "application.h"
#include "dialogs/decisionformdialog.h"
#include "utils/customcontextmenu.h"

#include <QDebug>
#include <QHeaderView>
#include <QSettings>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>

DecisionView::DecisionView(const QVariant &authorityId, QWidget *parent) :
    View(parent),
    ui(new Ui::DecisionView),
    m_authorityId(authorityId)
{
    ui->setupUi(this);

    initialize();
    restoreViewState();
}

DecisionView::~DecisionView()
{
    saveViewState();

    delete ui;
    delete m_table;
    delete m_paginator;
}

void DecisionView::initialize()
{
    m_table = new DecisionTable;
    ui->vL_data->insertWidget(0, m_table);

    m_paginator = new Paginator;
    ui->vL_paginator->addWidget(m_paginator);

    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DecisionView::selected);

    connect(m_table, &Table::onInsert, this, &DecisionView::insert);
    connect(m_table, &Table::onEdit, this,  &DecisionView::edit);
    connect(m_table, &Table::onRemove, this,  &DecisionView::remove);
    connect(m_table, &Table::onRefresh, this, &DecisionView::refresh);

    refresh();
}

void DecisionView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->splitter_data->restoreState(settings->value("Views/splitter_data").toByteArray());
    m_table->horizontalHeader()->restoreState(settings->value("Views/document_table").toByteArray());
}

void DecisionView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("splitter_data", ui->splitter_data->saveState());
    settings->setValue("document_table", m_table->horizontalHeader()->saveState());
    settings->endGroup();
}

void DecisionView::clear()
{
    m_table->clearContents();
    m_table->setRowCount(0);

    m_table->setColumnCount(m_headerLabels.length());
    m_table->setHorizontalHeaderLabels(m_headerLabels);
    // stretch last item
    m_table->horizontalHeader()->setSectionResizeMode(m_headerLabels.length() - 1, QHeaderView::Stretch);
}

void DecisionView::refresh()
{
    if (!m_authorityId.isValid())
        return;

    clear();

    QSqlQuery query;
    query.prepare("select id, number, name, comment from pad_decision where authority_id=?");
    query.bindValue(0, m_authorityId);
    query.exec();

    if(query.isActive()) {
        if(!query.size())
            return;

        int cols = query.record().count();

        while(query.next()) {
            m_table->setRowCount(m_table->rowCount() + 1);

            int row = query.at();
             for (int j = 0; j < cols; ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(j).toString());
                m_table->setItem(row, j, item);
             }
        }

        m_table->hideColumn(0); //id
    } else {
        qDebug() << query.lastError().text();

        QMessageBox::warning(this,
                tr("Documents"),
                tr("Could not fetch data."),
                QMessageBox::Ok);
    }

    updateTotal();
}

void DecisionView::edit()
{
    const QVariant id = m_table->item(m_table->currentRow(), 0)->data(Qt::DisplayRole);

    DecisionFormDialog dialog(m_authorityId, id);
    int res = dialog.exec();

    if(res == DecisionFormDialog::Accepted) {
        QSqlQuery query;
        query.prepare("update pad_decision SET authority_id=:authority_id, fund_id=:fund_id, inventory_id=:inventory_id, record_id=:record_id, doctype_id=:doctype_id, protocol_id=:protocol_id, number=:number, date=:date, name=:name, pages=:pages, protocol_page=:protocol_page, annexes=:annexes, content=:content, comment=:comment, access=:access where id=:id");
        query.bindValue(":id", dialog.getId());
        query.bindValue(":authority_id", dialog.getAuthority());
        query.bindValue(":fund_id", dialog.getFund());
        query.bindValue(":inventory_id", dialog.getInventory());
        query.bindValue(":record_id", dialog.getRecord());
        query.bindValue(":doctype_id", dialog.getDoctype());
        query.bindValue(":protocol_id", dialog.getProtocol());
        query.bindValue(":number", dialog.getNumber());
        query.bindValue(":date", dialog.getDate());
        query.bindValue(":name", dialog.getNumber());
        query.bindValue(":pages", dialog.getPages());
        query.bindValue(":protocol_page", dialog.getProtocolPage());
        query.bindValue(":annexes", dialog.getAnnexes());
        query.bindValue(":content", dialog.getContent());
        query.bindValue(":comment", dialog.getComment());
        query.bindValue(":access", dialog.getAccess());

        if(query.exec()) {
            refresh();
        } else {
            qDebug() << query.lastError().text();

            QMessageBox::warning(this,
                    tr("Documents"),
                    tr("Could not save data."),
                    QMessageBox::Ok);
        }
    }
}

void DecisionView::insert()
{
    DecisionFormDialog dialog(m_authorityId);
    int res = dialog.exec();

    if(res == DecisionFormDialog::Accepted) {
        QSqlQuery query;
        query.prepare("insert into pad_decision values (:id, :authority_id, :fund_id, :inventory_id, :record_id, :doctype_id, :protocol_id, :number, :date, :name, :pages, :protocol_page, :annexes, :content, :comment, :access)");
        query.bindValue(":id", QVariant());
        query.bindValue(":authority_id", dialog.getAuthority());
        query.bindValue(":fund_id", dialog.getFund());
        query.bindValue(":inventory_id", dialog.getInventory());
        query.bindValue(":record_id", dialog.getRecord());
        query.bindValue(":doctype_id", dialog.getDoctype());
        query.bindValue(":protocol_id", dialog.getProtocol());
        query.bindValue(":number", dialog.getNumber());
        query.bindValue(":date", dialog.getDate());
        query.bindValue(":name", dialog.getNumber());
        query.bindValue(":pages", dialog.getPages());
        query.bindValue(":protocol_page", dialog.getProtocolPage());
        query.bindValue(":annexes", dialog.getAnnexes());
        query.bindValue(":content", dialog.getContent());
        query.bindValue(":comment", dialog.getComment());
        query.bindValue(":access", dialog.getAccess());

        if(query.exec()) {
            refresh();
        } else {
            qDebug() << query.lastError().text();

            QMessageBox::warning(this,
                    tr("Documents"),
                    tr("Could not create item."),
                    QMessageBox::Ok);
        }
    }
}

void DecisionView::remove()
{
    int res = QMessageBox::critical(this,
        tr("Documents"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        const QVariant id = m_table->item(m_table->currentRow(), 0)->data(Qt::DisplayRole);
        QSqlQuery query;
        query.prepare("delete from pad_decision where id=?");
        query.bindValue(0, id);

        if(query.exec()) {
            refresh();
        } else {
            qDebug() << query.lastError().text();

            QMessageBox::warning(this,
                    tr("Documents"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}

void DecisionView::selected(const QItemSelection &, const QItemSelection &)
{
    int len = m_table->selectionModel()->selectedRows().length();

    if(!len)
        m_table->setCurrentIndex(QModelIndex());

    m_table->setInsertEnabled(len == 1);
    m_table->setEditEnabled(len > 0);
    m_table->setRemoveEnabled(len > 0);

    application->mainWindow()->action_edit->setEnabled(len == 1);
    application->mainWindow()->action_remove->setEnabled(len > 0);
}

void DecisionView::updateTotal()
{
    QSqlQuery query;
    query.prepare("select count(id) from pad_decision where authority_id=?");
    query.bindValue(0, m_authorityId);

    query.exec();
    if(query.isActive()) {
        query.first();

        m_total = query.value("count(id)").toInt();
        ui->lcdn_counter->display(m_total);
    }
}
