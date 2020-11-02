#ifndef DECISIONVIEW_H
#define DECISIONVIEW_H

#include "models/decisionmodel.h"
#include "models/decisionproxymodel.h"
#include "views/view.h"
#include "widgets/paginator.h"

#include <QWidget>
#include <QShortcut>

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

public slots:
    void edit();
    void insert();
    void refresh();
    void remove();

private:
    Ui::DecisionView *ui;
    Paginator *m_paginator;

    DecisionModel *m_model;
    DecisionProxyModel *m_proxyModel;

    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;
    QShortcut *refreshShortcut;

    void initialize() override;

    void contextMenu(const QPoint &pos);
    void setupShortcuts();
};

#endif // DECISIONVIEW_H
