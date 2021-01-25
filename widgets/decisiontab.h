#ifndef DECISIONTAB_H
#define DECISIONTAB_H

#include "tab.h"
#include "views/decisionview.h"

class DecisionTab : public Tab
{
    Q_OBJECT
public:
    DecisionTab(const QVariant &authorityId, QWidget *parent = nullptr);
    ~DecisionTab();

    DecisionView *view() { return m_view; };

private:
    DecisionView *m_view;
};

#endif // DECISIONTAB_H
