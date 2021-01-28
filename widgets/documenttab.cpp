#include "documenttab.h"

DocumentTab::DocumentTab(const QVariant &authorityId, QWidget *parent) : Tab(parent)
{
    m_view = new DocumentView;
    m_view->setAuthorityId(authorityId);
    m_view->refresh();

    layout()->addWidget(m_view);
}

DocumentTab::~DocumentTab()
{
    delete m_view;
}
