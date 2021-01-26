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

    m_table = new DecisionTable;
    m_paginator = new Paginator;

    initialize();
    restoreViewState();

    refresh(m_table->horizontalHeader()->sortIndicatorOrder());
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
    m_table->setSortingEnabled(false);
    m_table->horizontalHeader()->setSortIndicatorShown(true);
    m_table->setColumnCount(m_headerLabels.length());
    m_table->setHorizontalHeaderLabels(m_headerLabels);
    m_table->hideColumn(0);

    ui->vL_data->insertWidget(0, m_table);

    ui->vL_paginator->addWidget(m_paginator);

    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DecisionView::selected);

    connect(m_table, &Table::onInsert, this, &DecisionView::insert);
    connect(m_table, &Table::onEdit, this,  &DecisionView::edit);
    connect(m_table, &Table::onRemove, this,  &DecisionView::remove);
    connect(m_table, &Table::onRefresh, this, &DecisionView::refresh);

    // save header state dynamicly
    connect(m_table->horizontalHeader(), &QHeaderView::sectionResized, [=] { saveViewState(); } );
    connect(m_table->horizontalHeader(), &QHeaderView::sortIndicatorChanged, [=] { saveViewState(); } );
}

void DecisionView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->splitter_data->restoreState(settings->value("Views/splitter_data").toByteArray());
    m_table->horizontalHeader()->restoreState(settings->value("Views/document_table").toByteArray());

    connect(m_table->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &DecisionView::sort);
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

    // stretch last item
    m_table->horizontalHeader()->setSectionResizeMode(m_headerLabels.length() - 1, QHeaderView::Stretch);
}

void DecisionView::refresh(Qt::SortOrder order)
{
    if (!m_authorityId.isValid())
        return;

    clear();

    QString fieldName;
    switch (m_table->horizontalHeader()->sortIndicatorSection()) {
    case 1: fieldName = "cast(number as unsigned)"; break;
    case 2: fieldName = "name"; break;
    case 3: fieldName = "comment"; break;

    default: fieldName = "id"; break;
    }

    QString sort = fieldName + (order == Qt::AscendingOrder ? " asc" : " desc");

    QSqlQuery query;
    query.prepare(QString("select id, number, name, comment from pad_decision where authority_id=? order by %1").arg(sort));
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
        int row = m_table->currentRow();
        m_table->item(row, 0)->setText(dialog.getId().toString());
        m_table->item(row, 1)->setText(dialog.getNumber().toString());
        m_table->item(row, 2)->setText(dialog.getTitle().toString());
        m_table->item(row, 3)->setText(dialog.getComment().toString());
    }
}

void DecisionView::insert()
{
    DecisionFormDialog dialog(m_authorityId);
    int res = dialog.exec();

    if(res == DecisionFormDialog::Accepted) {
        m_table->insertRow(0);

        m_table->setItem(0, 0, new QTableWidgetItem(dialog.getId().toString()));
        m_table->setItem(0, 1, new QTableWidgetItem(dialog.getNumber().toString()));
        m_table->setItem(0, 2, new QTableWidgetItem(dialog.getTitle().toString()));
        m_table->setItem(0, 3, new QTableWidgetItem(dialog.getComment().toString()));

        m_table->selectRow(0);

        ui->lcdn_counter->display(m_total + 1);
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

void DecisionView::sort(int, Qt::SortOrder order)
{
    refresh(order);
}

void DecisionView::selected(const QItemSelection &, const QItemSelection &)
{
    int len = m_table->selectionModel()->selectedRows().length();

    if(!len)
        m_table->setCurrentIndex(QModelIndex());

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
