#ifndef DECISIONTAB_H
#define DECISIONTAB_H

#include "tab.h"
#include "models/decisionmodel.h"
#include "views/decisionview.h"

class DecisionTab : public Tab
{
    Q_OBJECT
public:
    DecisionTab(QWidget *parent = nullptr);
    ~DecisionTab();

    DecisionModel *model() { return m_model; };

private:
    DecisionView *m_view;
    DecisionModel *m_model;
};

#endif // DECISIONTAB_H
