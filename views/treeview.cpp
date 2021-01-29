#include "treeview.h"

#include <QDebug>

TreeView::TreeView(QWidget *parent) : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTreeView::EditKeyPressed);

    setSortingEnabled(true);

    setupShortcuts();

    connect(this, &QMenu::customContextMenuRequested, this, &TreeView::contextMenuRequested);
}

TreeView::~TreeView()
{
    delete m_insertShortcut;
    delete m_editShortcut;
    delete m_removeShortcut;
    delete m_refreshShortcut;
}

void TreeView::setupShortcuts()
{
    m_insertShortcut = new QShortcut(QKeySequence::New, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_insertShortcut->setEnabled(true);
    connect(m_insertShortcut, &QShortcut::activated, this, &TreeView::insertRow);

    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut->setEnabled(false);
    connect(m_editShortcut, &QShortcut::activated, this, &TreeView::editRow);

    m_removeShortcut = new QShortcut(QKeySequence::Delete, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut->setEnabled(false);
    connect(m_removeShortcut, &QShortcut::activated, this, &TreeView::removeRows);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut->setEnabled(true);
    connect(m_refreshShortcut, &QShortcut::activated, this, &TreeView::refresh);
}

void TreeView::contextMenuRequested(const QPoint &)
{
    BaseContextMenu menu(BaseContextMenu::Insert | BaseContextMenu::Edit | BaseContextMenu::Remove | BaseContextMenu::Refresh);

    QAction *insertAction = menu.action(BaseContextMenu::Insert);
    insertAction->setShortcut(m_insertShortcut->key());
    insertAction->setEnabled(m_insertEnabled);
    connect(insertAction, &QAction::triggered, this, &TreeView::insertRow);

    QAction *editAction = menu.action(BaseContextMenu::Edit);
    editAction->setShortcut(m_editShortcut->key());
    editAction->setEnabled(m_editEnabled);
    connect(editAction, &QAction::triggered, this, &TreeView::editRow);

    QAction *removeAction = menu.action(BaseContextMenu::Remove);
    removeAction->setShortcut(m_removeShortcut->key());
    removeAction->setEnabled(m_removeEnabled);
    connect(removeAction, &QAction::triggered, this, &TreeView::removeRows);

    QAction *refreshAction = menu.action(BaseContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    refreshAction->setEnabled(m_refreshEnabled);
    connect(refreshAction, &QAction::triggered, this, &TreeView::refresh);

    contextMenu(menu);
    menu.exec(QCursor().pos());
}

void TreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &diselected)
{
    setInsertEnabled(true);
    setEditEnabled(!selected.isEmpty());
    setRemoveEnabled(!selected.isEmpty());
    setRefreshEnabled(true);

    QTreeView::selectionChanged(selected, diselected);
}

void TreeView::editRow()
{
    edit(currentIndex());
}

void TreeView::setInsertEnabled(bool ok)
{
    m_insertEnabled = ok;
    if(isShortcutsEnabled())
        m_insertShortcut->setEnabled(ok);
}

void TreeView::setEditEnabled(bool ok)
{
    m_editEnabled = ok;
    if(isShortcutsEnabled())
        m_editShortcut->setEnabled(ok);
}

void TreeView::setRemoveEnabled(bool ok)
{
    m_removeEnabled = ok;
    if(isShortcutsEnabled())
        m_removeShortcut->setEnabled(ok);
}

void TreeView::setRefreshEnabled(bool ok)
{
    m_refreshEnabled = ok;
    if(isShortcutsEnabled())
        m_refreshShortcut->setEnabled(ok);
}

void TreeView::setContextMenuEnabled(bool ok)
{
    m_contextMenuEnabled = ok;
    if(!ok)
        disconnect(this, &QMenu::customContextMenuRequested, this, &TreeView::contextMenuRequested);
}

void TreeView::setShortcutsEnabled(bool ok)
{
    m_contextMenuEnabled = ok;
    if(!ok) {
        m_insertShortcut->setEnabled(false);
        m_editShortcut->setEnabled(false);
        m_removeShortcut->setEnabled(false);
        m_refreshShortcut->setEnabled(false);
    }
}
