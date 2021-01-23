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
    explicit DecisionView(QWidget *parent = nullptr);
    ~DecisionView();

    void restoreViewState() override;
    void saveViewState() override;

    DecisionTable *table() { return m_table; };

public slots:
    void edit();
    void insert();
    void refresh();
    void remove();

private:
    Ui::DecisionView *ui;
    DecisionTable *m_table;
    Paginator *m_paginator;

    void initialize() override;

private slots:
    void contextMenu(CustomContextMenu &menu);
    void selected(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // DECISIONVIEW_H
