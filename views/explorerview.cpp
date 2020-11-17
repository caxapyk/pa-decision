#include "explorerview.h"
#include "ui_explorerview.h"

ExplorerView::ExplorerView(QWidget *parent) :
    View(parent),
    ui(new Ui::ExplorerView)
{
    ui->setupUi(this);
}

ExplorerView::~ExplorerView()
{
    delete ui;
}

void ExplorerView::createTab(View *view, const QString &title)
{
    QWidget *tab = new QWidget;

    ui->tW_panel->addTab(tab, title);
}
