#include "navigatorview.h"
#include "ui_navigatorview.h"

NavigatorView::NavigatorView(QWidget *parent) :
    View(parent),
    ui(new Ui::NavigatorView)
{
    ui->setupUi(this);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &NavigatorView::tabSwitch);
    tabSwitch(0);
}

NavigatorView::~NavigatorView()
{
    delete ui;
    delete m_authorityView;
    delete m_collectionView;
}

void NavigatorView::restoreViewState()
{
    // restore views state on tabSwitch
}

void NavigatorView::saveViewState()
{
    if(m_authorityView != nullptr)
        m_authorityView->saveViewState();
    if(m_collectionView != nullptr)
        m_collectionView->saveViewState();
}

void NavigatorView::tabSwitch(int index)
{
    switch (index) {
    case NavigatorView::TabAuthority:
        if(m_authorityView == nullptr) {
            m_authorityView = new AuthorityView(this);
            ui->tab_authority->layout()->addWidget(m_authorityView);
            m_authorityView->restoreViewState();
        }
        break;
    case NavigatorView::TabCollection:
        if(m_collectionView == nullptr) {
            m_collectionView = new CollectionView(this);
            ui->tab_collection->layout()->addWidget(m_collectionView);
            m_collectionView->restoreViewState();
        }
        break;
    case NavigatorView::TabSearch:
        break;

    }
}
