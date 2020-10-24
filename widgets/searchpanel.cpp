#include "searchpanel.h"
#include "ui_searchpanel.h"

SearchPanel::SearchPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchPanel)
{
    ui->setupUi(this);
    ui->horizontalLayout->setAlignment(Qt::AlignRight);
}

SearchPanel::~SearchPanel()
{
    delete ui;
}

void SearchPanel::setFocus()
{
    ui->lE_search->setFocus();
}
