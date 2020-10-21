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
    m_authorityModel = new AuthorityTreeModel();
    m_authorityModel->select();
    ui->tV_tree->setModel(m_authorityModel);
    ui->tV_tree->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_tree, &QMenu::customContextMenuRequested, this, &AuthorityView::showContextMenu);
}


void AuthorityView::showContextMenu()
{

}
