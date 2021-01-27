#ifndef DECISIONVIEW_H
#define DECISIONVIEW_H

#include "models/decisionmodel.h"
#include "models/decisionproxymodel.h"
#include "widgets/basecontextmenu.h"
#include "views/view.h"
#include "widgets/documenttable.h"
#include "widgets/paginator.h"

#include <QShortcut>
#include <QWidget>

namespace Ui {
class DecisionView;
}

class DecisionView : public View
{
    Q_OBJECT

public:
    explicit DecisionView(const QVariant &authorityId, QWidget *parent = nullptr);
    ~DecisionView();

    void restoreViewState() override;
    void saveViewState() override;

    QVariant authorityId() const { return m_authorityId; };

public slots:
    void edit();
    void insert();
    void refresh(Qt::SortOrder order = Qt::AscendingOrder);
    void remove();

private:
    Ui::DecisionView *ui;
    DocumentTable *m_table;
    Paginator *m_paginator;

    QVariant m_authorityId;
    int m_total = -1;

    void initialize() override;
    void updateTotal();

private slots:
    void onSelectionChanged();
    void sort(int section, Qt::SortOrder order);
};

#endif // DECISIONVIEW_H
