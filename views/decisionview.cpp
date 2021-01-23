#include "decisionview.h"
#include "ui_decisionview.h"

#include "application.h"
#include "dialogs/decisionformdialog.h"
#include "utils/customcontextmenu.h"

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QSqlQuery>

DecisionView::DecisionView(QWidget *parent) :
    View(parent),
    ui(new Ui::DecisionView)
{
    ui->setupUi(this);

    m_table = new DecisionTable;
    ui->vL_data->insertWidget(0, m_table);

    m_paginator = new Paginator;
    ui->vL_paginator->addWidget(m_paginator);

    restoreViewState();
    initialize();
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
    connect(m_table, &Table::onContextMenuRequested, this, &DecisionView::contextMenu);
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DecisionView::selected);

    connect(m_table, &Table::onInsert, this, &DecisionView::insert);
    connect(m_table, &Table::onEdit, this,  &DecisionView::edit);
    connect(m_table, &Table::onRemove, this,  &DecisionView::remove);
    connect(m_table, &Table::onRefresh, this, &DecisionView::refresh);
}

void DecisionView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->splitter_data->restoreState(settings->value("Views/splitter_data").toByteArray());
}

void DecisionView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("splitter_data", ui->splitter_data->saveState());
    settings->endGroup();
}

void DecisionView::contextMenu(CustomContextMenu &menu)
{
    menu.exec(QCursor().pos());
}

void DecisionView::edit()
{
    //QVariant id = ui->tV_decision->currentIndex().siblingAtColumn(0).data();
    //qDebug() << "id passed: " << id;

    //DecisionFormDialog dialog(id);
    //int res = dialog.exec();

    //if(res == DecisionFormDialog::Accepted) {
    //    refresh();
    //}
}

void DecisionView::insert()
{
    DecisionFormDialog dialog(table()->authorityId());
    int res = dialog.exec();

    if(res == DecisionFormDialog::Accepted) {
        refresh();
    }
}

void DecisionView::refresh()
{
    //
}

void DecisionView::remove()
{
    //
}

void DecisionView::selected(const QItemSelection &, const QItemSelection &)
{
    int len = m_table->selectionModel()->selectedRows().length();

    if(!len)
        m_table->setCurrentIndex(QModelIndex());

    m_table->setInsertEnabled(len == 1);
    m_table->setRemoveEnabled(len > 0);

    application->mainWindow()->action_edit->setEnabled(len == 1);
    application->mainWindow()->action_remove->setEnabled(len > 0);
}
