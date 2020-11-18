#include "decisionview.h"
#include "ui_decisionview.h"

#include "application.h"
#include "dialogs/decisionnewdialog.h"
#include "widgets/customcontextmenu.h"

#include <QSettings>

DecisionView::DecisionView(QWidget *parent) :
    View(parent),
    ui(new Ui::DecisionView)
{
    ui->setupUi(this);
    setupShortcuts();

    m_paginator = new Paginator;
    ui->vL_paginator->addWidget(m_paginator);

    initialize();
    restoreViewState();
}

DecisionView::~DecisionView()
{
    saveViewState();

    delete ui;
    delete m_paginator;
    delete m_model;
    delete m_proxyModel;

    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
    delete refreshShortcut;
}

void DecisionView::initialize()
{
    m_model = new DecisionReadModel;

    m_proxyModel = new DecisionProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_decision->setModel(m_proxyModel);
    ui->tV_decision->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_decision, &QMenu::customContextMenuRequested, this, &DecisionView::contextMenu);
    connect(ui->tV_decision->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DecisionView::selected);
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

void DecisionView::contextMenu(const QPoint &)
{
    CustomContextMenu menu(CustomContextMenu::All);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(true);
    connect(insertAction, &QAction::triggered, this, &DecisionView::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &DecisionView::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &DecisionView::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    refreshShortcut->setEnabled(true);
    connect(refreshAction, &QAction::triggered, this, &DecisionView::refresh);

    menu.exec(QCursor().pos());
}

void DecisionView::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    insertShortcut->setEnabled(true);
    connect(insertShortcut, &QShortcut::activated, this, &DecisionView::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    editShortcut->setEnabled(false);
    connect(editShortcut, &QShortcut::activated, this, &DecisionView::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    removeShortcut->setEnabled(false);
    connect(removeShortcut, &QShortcut::activated, this, &DecisionView::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    setEnabled(true);
    connect(refreshShortcut, &QShortcut::activated, this, &DecisionView::refresh);
}

void DecisionView::edit()
{

}

void DecisionView::insert()
{
    DecisionNewDialog dialog;
    int res = dialog.exec();

    if(res == DecisionNewDialog::Accepted) {
        // do smth
    }
}

void DecisionView::refresh()
{
    ui->tV_decision->selectionModel()->clearSelection();

    m_proxyModel->invalidate();
    m_model->select();
}

void DecisionView::remove()
{

}

void DecisionView::selected(const QItemSelection &, const QItemSelection &)
{
    int len = ui->tV_decision->selectionModel()->selectedRows().length();

    editShortcut->setEnabled(len == 1);
    removeShortcut->setEnabled(len > 0);

    application->mainWindow()->action_edit->setEnabled(len == 1);
    application->mainWindow()->action_remove->setEnabled(len > 0);
}
