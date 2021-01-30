#ifndef DECISIONTAB_H
#define DECISIONTAB_H

#include "tab.h"
#include "views/documentview.h"
#include "widgets/navpanel.h"
#include "widgets/paginator.h"

#include <QTextBrowser>
#include <QDockWidget>
#include <QSplitter>

class DocumentTab : public Tab
{
    Q_OBJECT
public:
    DocumentTab(const QVariant &authorityId, QWidget *parent = nullptr);
    ~DocumentTab();

    DocumentView *view() { return m_view; };

private:
    DocumentView *m_view;
    Paginator *m_paginator;
    NavPanel *m_panel;
    QTextBrowser *m_browser;
    QSplitter *m_splitter;
    QDockWidget *m_dock;

private slots:
    void rangeSelected(const QList<QTableWidgetSelectionRange> &ranges);
    void backward();
    void toward();

    void openBrowser(int row);

    void restoreState();
    void saveState();
};

#endif // DECISIONTAB_H
