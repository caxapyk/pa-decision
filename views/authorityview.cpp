#include "application.h"

#include "authorityview.h"
#include "ui_authorityview.h"

AuthorityView::AuthorityView(QWidget *parent) :
    View(parent),
    ui(new Ui::AuthorityView)
{
    ui->setupUi(this);

    initialize();
}

AuthorityView::~AuthorityView()
{
    delete ui;
    delete m_authorityModel;
}

void AuthorityView::initialize()
{
    m_authorityModel = new AuthorityModel();
    ui->tV_tree->setModel(m_authorityModel);
}
