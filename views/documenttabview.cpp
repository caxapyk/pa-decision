#include "documenttabview.h"
#include "application.h"
#include "utils/htmltemplate.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

DocumentTabView::DocumentTabView(const QVariant &authorityId, QWidget *parent) : Tab(parent)
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
    connect(m_view, &DocumentView::cellClicked, this, &DocumentTabView::clicked);
    connect(m_view, &DocumentView::cellDoubleClicked, this, &DocumentTabView::openBrowser);
    connect(m_view, &DocumentView::viewDocument, this, &DocumentTabView::openBrowser);
    connect(m_view, &DocumentView::refreshed, this, &DocumentTabView::refreshed);

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

DocumentTabView::~DocumentTabView()
{
    saveState();

    delete m_view;
    delete m_paginator;
    delete m_panel;
    delete m_browser;
    delete m_splitter;
}

void DocumentTabView::saveState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Widgets");
    settings->setValue("splitter_document", m_splitter->saveState());
    settings->endGroup();
}

void DocumentTabView::restoreState()
{
    QSettings* settings = application->applicationSettings();
    m_splitter->restoreState(settings->value("Widgets/splitter_document").toByteArray());

    switch (m_splitter->orientation()) {
        case Qt::Horizontal: m_panel->setHorizontalChecked(); break;
        case Qt::Vertical: m_panel->setVerticalChecked(); break;
    }
}

void DocumentTabView::refreshed()
{
    int page = m_view->page();
    int total = m_view->totalPages();

    m_paginator->setCurrentPage(page + 1);
    m_paginator->setTotalPages(total);
    m_paginator->setPreviousEnabled(page > 0);
    m_paginator->setNextEnabled(page < total - 1);
}

bool DocumentTabView::isDockOpen()
{
    return m_dock->isVisible();
}

void DocumentTabView::clicked()
{
    const QList<QTableWidgetSelectionRange> &ranges = m_view->selectedRanges();

    if(!ranges.isEmpty()) {
        m_paginator->setBackwardEnabled(ranges.first().topRow() > 0);
        m_paginator->setTowardEnabled(ranges.last().bottomRow() < m_view->rowCount() - 1);
    } else {
        m_paginator->setBackwardEnabled(false);
    }
}

void DocumentTabView::openBrowser(int row)
{
    const QVariant id = m_view->item(row, 0)->data(Qt::DisplayRole);
    m_dock->setWindowTitle("ID" + id.toString());

    HtmlTemplate templ("templates/documentform");

    QSqlQuery query;
    query.prepare(" \
            select \
            pad_decision.id, \
            pad_authority.name, \
            pad_doctype.name, \
            pad_decision.date, \
            pad_decision.number, \
            pad_decision.name, \
            pad_fund.number, \
            pad_inventory.number, \
            pad_record.number, \
            CONCAT(\"№\", pad_protocol.number, \" from \", pad_protocol.date) AS protocol, \
            pad_decision.pages, \
            pad_decision.annexes, \
            pad_decision.protocol_page, \
            pad_decision.content, \
            pad_decision.comment, \
            pad_decision.access \
            from pad_decision \
            left join pad_authority ON pad_decision.authority_id=pad_authority.id \
            left join pad_doctype ON pad_decision.doctype_id=pad_doctype.id \
            left join pad_fund ON pad_decision.fund_id=pad_fund.id \
            left join pad_inventory ON pad_decision.inventory_id=pad_inventory.id \
            left join pad_record ON pad_decision.record_id=pad_record.id \
            left join pad_protocol ON pad_decision.protocol_id=pad_protocol.id \
            where pad_decision.id=?");
    query.bindValue(0, id);
    query.exec();
    query.first();

    QSqlQuery query_s;
    query_s.prepare(" \
            select \
            pad_subjects.id, \
            pad_subjtype.name, \
            pad_subjects.name, \
            pad_subjects.addresses, \
            pad_subjects.pages, \
            pad_subjects.descr \
            from pad_subjects \
            left join pad_subjtype ON pad_subjects.subjtype_id=pad_subjtype.id \
            where pad_subjects.decision_id=?");
    query_s.bindValue(0, id);
    query_s.exec();

    if (query.isActive() && query_s.isActive()) {
        templ.set("id", query.value("pad_decision.id"));
        templ.set("authority_name", query.value("pad_authority.name"));
        templ.set("type", query.value("pad_doctype.name"));
        templ.set("name", query.value("pad_decision.name"));
        templ.set("date", query.value("pad_decision.date"));
        templ.set("fund_number", query.value("pad_fund.number"));
        templ.set("inventory_number", query.value("pad_inventory.number"));
        templ.set("record_number", query.value("pad_record.number"));
        templ.set("protocol", !query.value("protocol").isNull() ?
                      QString(" %1%2")
                      .arg(!query.value("protocol").isNull() ? query.value("protocol").toString() : QString())
                      .arg(!query.value("protocol_page").isNull() ? QString(" (%1)").arg(query.value("protocol_page").toString()) : QString()) : QVariant(tr("Not set")));
        templ.set("pages", !query.value("pad_decision.pages").isNull() ? query.value("pad_decision.pages") : QVariant(tr("Not set")));
        templ.set("annexes", query.value("pad_decision.annexes").toBool() ? tr("Yes") : tr("No"));
        templ.set("content", !query.value("pad_decision.content").isNull() ? query.value("pad_decision.content") : QVariant(tr("Not set")));
        templ.set("comment", !query.value("pad_decision.comment").isNull() ? query.value("pad_decision.comment") : QVariant(tr("Not set")));

        QString rows;
        int i = 1;
        while(query_s.next()) {
            rows.append(QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td></tr>")
                .arg(i)
                .arg(query_s.value("pad_subjtype.name").toString())
                .arg(query_s.value("pad_subjects.name").toString())
                .arg(query_s.value("pad_subjects.addresses").toString())
                .arg(query_s.value("pad_subjects.pages").toString())
                .arg(query_s.value("pad_subjects.descr").toString())
            );
            ++i;
        }

        templ.set("rows", !rows.isEmpty() ? QVariant(rows) : QVariant());

        m_browser->setHtml(templ.render()->toHtml());
    } else {
        qDebug () << query.lastError().text();
        qDebug () << query_s.lastError().text();
    }

    m_dock->show();
}

void DocumentTabView::print()
{

}
