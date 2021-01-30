#include "documenttab.h"
#include "application.h"

#include <QDebug>

DocumentTab::DocumentTab(const QVariant &authorityId, QWidget *parent) : Tab(parent)
{
    _layout()->setSpacing(0);

    m_panel = new NavPanel;
    connect(m_panel, &NavPanel::orientationChanged, this, [=] (Qt::Orientation orient) {
        m_splitter->setOrientation(orient);
    });

    m_paginator = new Paginator;
    m_panel->_layout()->insertWidget(0, m_paginator);
    connect(m_paginator, &Paginator::backward, this, &DocumentTab::backward);
    connect(m_paginator, &Paginator::toward, this, &DocumentTab::toward);

    _layout()->addWidget(m_panel);

    m_splitter = new QSplitter;
    m_splitter->setOrientation(Qt::Vertical);

    m_view = new DocumentView;
    m_view->setAuthorityId(authorityId);
    connect(m_view, &DocumentView::totalChanged, m_panel, &NavPanel::setTotal);
    connect(m_view, &DocumentView::_selected, this, &DocumentTab::rangeSelected);
    connect(m_view, &DocumentView::cellDoubleClicked, this, &DocumentTab::openBrowser);
    m_view->refresh();

    m_splitter->addWidget(m_view);

    m_dock = new QDockWidget;
    m_dock->setFeatures(QDockWidget::DockWidgetClosable);
    m_dock->hide();

    m_browser = new QTextBrowser;
    m_browser->setMinimumHeight(150);

    m_dock->setWidget(m_browser);
    m_splitter->addWidget(m_dock);

    _layout()->addWidget(m_splitter);

    restoreState();
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

void DocumentTab::rangeSelected(const QList<QTableWidgetSelectionRange> &ranges)
{
    if(!ranges.isEmpty()) {
        m_paginator->setBackwardEnabled(ranges.first().topRow() > 0);
        m_paginator->setTowardEnabled(ranges.last().bottomRow() < m_view->rowCount() - 1);
    } else {
        m_paginator->setBackwardEnabled(false);
    }
}

void DocumentTab::backward()
{
    const QList<QTableWidgetSelectionRange> range = m_view->selectedRanges();

    int row = range.isEmpty() ? 1 : range.first().topRow();
    int prev = row - 1;
    m_view->selectRow(prev);

    openBrowser(prev);
}

void DocumentTab::toward()
{
    const QList<QTableWidgetSelectionRange> range = m_view->selectedRanges();

    int row = range.isEmpty() ? -1 : range.last().bottomRow();
    int next = row + 1;
    m_view->selectRow(next);

    openBrowser(next);
}

void DocumentTab::openBrowser(int row)
{
    const QVariant id  = QVariant(m_view->item(row, 0)->text());

    m_dock->setWindowTitle("Document card №" + id.toString());
    m_browser->setText("Document with ID: " + id.toString());

    m_dock->show();
}
