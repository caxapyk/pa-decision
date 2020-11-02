#ifndef DECISIONVIEW_H
#define DECISIONVIEW_H

#include "models/decisionmodel.h"
#include "models/decisionproxymodel.h"
#include "views/view.h"
#include "widgets/paginator.h"

#include <QWidget>

namespace Ui {
class DecisionView;
}

class DecisionView : public View
{
    Q_OBJECT

public:
    explicit DecisionView(QWidget *parent = nullptr);
    ~DecisionView();

    void restoreViewState() override;
    void saveViewState() override;

private:
    Ui::DecisionView *ui;
    Paginator *m_paginator;

    DecisionModel *m_model;
    DecisionProxyModel *m_proxyModel;

    void initialize() override;
};

#endif // DECISIONVIEW_H
