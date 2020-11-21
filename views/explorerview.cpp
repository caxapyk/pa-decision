#include "explorerview.h"
#include "ui_explorerview.h"

#include "application.h"
#include "widgets/tab.h"

#include <QDebug>

ExplorerView::ExplorerView(QWidget *parent) :
    View(parent),
    ui(new Ui::ExplorerView)
{
    ui->setupUi(this);
    connect(ui->tW_panel, &QTabWidget::tabCloseRequested, this, &ExplorerView::closeTab);
}

ExplorerView::~ExplorerView()
{
    delete ui;
}

void ExplorerView::createTab(Tab *tab, const QString &title, QIcon icon)
{
    ui->tW_panel->addTab(tab, icon, title);
    ui->tW_panel->setCurrentIndex(ui->tW_panel->count() - 1);

    application->mainWindow()->action_new->setEnabled(true);
    application->mainWindow()->action_refresh->setEnabled(true);
}

void ExplorerView::closeTab(int index)
{
    delete ui->tW_panel->widget(index);

    if(ui->tW_panel->count() == 0) {
        application->mainWindow()->action_new->setDisabled(true);
        application->mainWindow()->action_refresh->setDisabled(true);
    }
}

Tab *ExplorerView::currentTab()
{
    int i = ui->tW_panel->currentIndex();
    Tab *tab = dynamic_cast<Tab*>(ui->tW_panel->widget(i));

    return tab;
}
