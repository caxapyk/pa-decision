#include "decisionview.h"
#include "ui_decisionview.h"

#include "application.h"
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
    m_model = new DecisionModel;
    m_model->select();

    m_proxyModel = new DecisionProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_decision->setModel(m_proxyModel);
    ui->tV_decision->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_decision, &QMenu::customContextMenuRequested, this, &DecisionView::contextMenu);
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

    //QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    //refreshAction->setShortcut(m_refreshShortcut->key());
    //connect(refreshAction, &QAction::triggered, this, [=] {
    ///    switchModel(ui->cB_collection->currentIndex());
    //});

    menu.exec(QCursor().pos());
}

void DecisionView::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &DecisionView::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &DecisionView::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &DecisionView::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &DecisionView::refresh);
}

void DecisionView::edit()
{

}

void DecisionView::insert()
{

}

void DecisionView::refresh()
{

}

void DecisionView::remove()
{

}
