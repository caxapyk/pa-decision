#include "application.h"

#include "decisionview.h"
#include "ui_decisionview.h"

#include <QSettings>

DecisionView::DecisionView(QWidget *parent) :
    View(parent),
    ui(new Ui::DecisionView)
{
    ui->setupUi(this);

    initialize();
    restoreViewState();
}

DecisionView::~DecisionView()
{
    delete ui;
}

void DecisionView::initialize()
{

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
