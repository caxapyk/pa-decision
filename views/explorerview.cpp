#include "explorerview.h"
#include "ui_explorerview.h"

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
}

void ExplorerView::closeTab(int index)
{
    delete ui->tW_panel->widget(index);
}
