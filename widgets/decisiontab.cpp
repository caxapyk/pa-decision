#include "decisiontab.h"

DecisionTab::DecisionTab(const QVariant &authorityId, QWidget *parent) : Tab(parent)
{
    m_view = new DecisionView(authorityId);
    layout()->addWidget(m_view);
}

DecisionTab::~DecisionTab()
{
    delete m_view;
}
