#include "decisionview.h"
#include "ui_decisionview.h"

#include "application.h"

#include <QSettings>

DecisionView::DecisionView(QWidget *parent) :
    View(parent),
    ui(new Ui::DecisionView)
{
    ui->setupUi(this);

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
}

void DecisionView::initialize()
{
    m_model = new DecisionModel;
    m_model->select();

    m_proxyModel = new DecisionProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_decision->setModel(m_proxyModel);
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
