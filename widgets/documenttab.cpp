#include "documenttab.h"
#include "application.h"

#include <QDebug>

DocumentTab::DocumentTab(const QVariant &authorityId, QWidget *parent) : Tab(parent)
{
    _layout()->setSpacing(3);

    m_panel = new NavPanel;
    connect(m_panel, &NavPanel::orientationChanged, this, [=] (Qt::Orientation orient) {
        m_splitter->setOrientation(orient);
    });

    m_paginator = new Paginator;
    m_panel->_layout()->insertWidget(0, m_paginator);

    _layout()->addWidget(m_panel);

    m_splitter = new QSplitter;
    m_splitter->setOrientation(Qt::Vertical);

    m_view = new DocumentView;
    m_view->setAuthorityId(authorityId);
    connect(m_view, &DocumentView::totalChanged, m_panel, &NavPanel::setTotal);
    connect(m_view, &DocumentView::cellClicked, this, &DocumentTab::clicked);
    connect(m_view, &DocumentView::cellDoubleClicked, this, &DocumentTab::openBrowser);
    connect(m_view, &DocumentView::refreshed, this, &DocumentTab::refreshed);

    connect(m_paginator, &Paginator::previousPage, m_view, &DocumentView::previousPage);
    connect(m_paginator, &Paginator::nextPage, m_view, &DocumentView::nextPage);
    connect(m_paginator, &Paginator::perPageChanged, m_view, &DocumentView::perPage);
    connect(m_paginator, &Paginator::backward, m_view, &DocumentView::backward);
    connect(m_paginator, &Paginator::toward, m_view, &DocumentView::toward);
    connect(m_paginator, &Paginator::gotoPage, m_view, &DocumentView::gotoPage);

    m_view->refresh();

    m_splitter->addWidget(m_view);

    m_dock = new QDockWidget;
    m_dock->setFeatures(QDockWidget::DockWidgetClosable);
    connect(m_dock, &QDockWidget::visibilityChanged, this, [=](bool visible){
        application->mainWindow()->printAction()->setEnabled(visible);
    });
    m_dock->hide();

    m_browser = new QTextBrowser;
    m_browser->setMinimumHeight(150);

    m_dock->setWidget(m_browser);
    m_splitter->addWidget(m_dock);

    _layout()->addWidget(m_splitter);

    restoreState();

    connect(m_paginator, &Paginator::backward, this, [=] {
        openBrowser(m_view->selectedRanges().first().topRow());
    });
    connect(m_paginator, &Paginator::toward, this, [=] {
                openBrowser(m_view->selectedRanges().last().bottomRow());
    });
}

DocumentTab::~DocumentTab()
{
    saveState();

    delete m_view;
    delete m_paginator;
    delete m_panel;
    delete m_browser;
    delete m_splitter;
}

void DocumentTab::saveState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Widgets");
    settings->setValue("splitter_document", m_splitter->saveState());
    settings->endGroup();
}

void DocumentTab::restoreState()
{
    QSettings* settings = application->applicationSettings();
    m_splitter->restoreState(settings->value("Widgets/splitter_document").toByteArray());

    switch (m_splitter->orientation()) {
        case Qt::Horizontal: m_panel->setHorizontalChecked(); break;
        case Qt::Vertical: m_panel->setVerticalChecked(); break;
    }
}

void DocumentTab::refreshed()
{
    int page = m_view->page();
    int total = m_view->totalPages();

    m_paginator->setCurrentPage(page + 1);
    m_paginator->setTotalPages(total);
    m_paginator->setPreviousEnabled(page > 0);
    m_paginator->setNextEnabled(page < total - 1);
}

bool DocumentTab::isDockOpen()
{
    return m_dock->isVisible();
}

void DocumentTab::clicked()
{
    const QList<QTableWidgetSelectionRange> &ranges = m_view->selectedRanges();

    if(!ranges.isEmpty()) {
        m_paginator->setBackwardEnabled(ranges.first().topRow() > 0);
        m_paginator->setTowardEnabled(ranges.last().bottomRow() < m_view->rowCount() - 1);
    } else {
        m_paginator->setBackwardEnabled(false);
    }
}

void DocumentTab::openBrowser(int row)
{
    const QString title  = m_view->item(row, 2)->text();

    m_dock->setWindowTitle(title);
    m_browser->setText("Document with ID: " + m_view->item(row, 0)->text());

    m_dock->show();
}
