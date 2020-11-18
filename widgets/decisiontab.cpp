#include "decisiontab.h"

DecisionTab::DecisionTab(QWidget *parent) : Tab(parent)
{
    m_view = new DecisionView;
    layout()->addWidget(m_view);

    m_model = new DecisionModel;
}

DecisionTab::~DecisionTab()
{
    delete m_view;
    delete m_model;
}
