#ifndef DECISIONVIEW_H
#define DECISIONVIEW_H

#include "models/decisionmodel.h"
#include "models/decisionproxymodel.h"
#include "utils/customcontextmenu.h"
#include "views/view.h"
#include "widgets/decisiontable.h"
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
    DecisionTable *m_table;
    Paginator *m_paginator;

    QStringList m_headerLabels = {tr("ID"), tr("Number"), tr("Name"), tr("Comment")};
    QVariant m_authorityId;
    int m_total = -1;

    void initialize() override;
    void clear();
    void updateTotal();

private slots:
    void selected(const QItemSelection &selected, const QItemSelection &deselected);
    void sort(int section, Qt::SortOrder order);
};

#endif // DECISIONVIEW_H
