#include "tablewidgetview.h"

#include <QDebug>

TableWidgetView::TableWidgetView(QWidget *parent) : QTableWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTableWidget::NoEditTriggers);
    setSortingEnabled(true);

    setupShortcuts();
    clearSelection();

    connect(this, &QMenu::customContextMenuRequested, this, &TableWidgetView::contextMenuRequested);
}

TableWidgetView::~TableWidgetView()
{
    delete m_insertShortcut;
    delete m_editShortcut;
    delete m_removeShortcut;
    delete m_refreshShortcut;
}

void TableWidgetView::setupShortcuts()
{
    m_insertShortcut = new QShortcut(QKeySequence::New, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_insertShortcut->setEnabled(true);
    connect(m_insertShortcut, &QShortcut::activated, this, &TableWidgetView::_insertRow);

    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut->setEnabled(false);
    connect(m_editShortcut, &QShortcut::activated, this, &TableWidgetView::editRow);

    m_removeShortcut = new QShortcut(QKeySequence::Delete, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut->setEnabled(false);
    connect(m_removeShortcut, &QShortcut::activated, this, &TableWidgetView::removeRows);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut->setEnabled(true);
    connect(m_refreshShortcut, &QShortcut::activated, this, &TableWidgetView::refresh);
}

void TableWidgetView::contextMenuRequested(const QPoint &)
{
    BaseContextMenu menu(BaseContextMenu::Insert | BaseContextMenu::Edit | BaseContextMenu::Remove | BaseContextMenu::Refresh);

    QAction *insertAction = menu.action(BaseContextMenu::Insert);
    insertAction->setShortcut(m_insertShortcut->key());
    insertAction->setEnabled(m_insertEnabled);
    connect(insertAction, &QAction::triggered, this, &TableWidgetView::_insertRow);

    QAction *editAction = menu.action(BaseContextMenu::Edit);
    editAction->setShortcut(m_editShortcut->key());
    editAction->setEnabled(m_editEnabled);
    connect(editAction, &QAction::triggered, this, &TableWidgetView::editRow);

    QAction *removeAction = menu.action(BaseContextMenu::Remove);
    removeAction->setShortcut(m_removeShortcut->key());
    removeAction->setEnabled(m_removeEnabled);
    connect(removeAction, &QAction::triggered, this, &TableWidgetView::removeRows);

    QAction *refreshAction = menu.action(BaseContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    refreshAction->setEnabled(m_refreshEnabled);
    connect(refreshAction, &QAction::triggered, this, &TableWidgetView::refresh);

    contextMenu(menu);
    menu.exec(QCursor().pos());
}

void TableWidgetView::_clear()
{
    clearContents();
    setRowCount(0);
}

void TableWidgetView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    setInsertEnabled(true);
    setEditEnabled(!selected.isEmpty());
    setRemoveEnabled(!selected.isEmpty());
    setRefreshEnabled(true);

    QTableView::selectionChanged(selected, deselected);
}

void TableWidgetView::setInsertEnabled(bool ok)
{
    m_insertEnabled = ok;
    if(isShortcutsEnabled())
        m_insertShortcut->setEnabled(ok);
}

void TableWidgetView::setEditEnabled(bool ok)
{
    m_editEnabled = ok;
    if(isShortcutsEnabled())
        m_editShortcut->setEnabled(ok);
}

void TableWidgetView::setRemoveEnabled(bool ok)
{
    m_removeEnabled = ok;
    if(isShortcutsEnabled())
        m_removeShortcut->setEnabled(ok);
}

void TableWidgetView::setRefreshEnabled(bool ok)
{
    m_refreshEnabled = ok;
    if(isShortcutsEnabled())
        m_refreshShortcut->setEnabled(ok);
}

void TableWidgetView::setContextMenuEnabled(bool ok)
{
    m_contextMenuEnabled = ok;
    if(!ok)
        disconnect(this, &QMenu::customContextMenuRequested, this, &TableWidgetView::contextMenuRequested);
}

void TableWidgetView::setShortcutsEnabled(bool ok)
{
    m_contextMenuEnabled = ok;
    if(!ok) {
        m_insertShortcut->setEnabled(false);
        m_editShortcut->setEnabled(false);
        m_removeShortcut->setEnabled(false);
        m_refreshShortcut->setEnabled(false);
    }
}
