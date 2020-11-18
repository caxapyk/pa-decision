#include "decisiontab.h"

DecisionTab::DecisionTab(QWidget *parent) : Tab(parent)
{
    m_view = new DecisionView;
    layout()->addWidget(m_view);
}

DecisionTab::~DecisionTab()
{
    delete m_view;
}
