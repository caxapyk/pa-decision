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
    connect(m_insertShortcut, &QShortcut::activated, this, [=] { insertRow(currentIndex()); });

    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut->setEnabled(false);
    connect(m_editShortcut, &QShortcut::activated, this, [=] { editRow(currentIndex()); });

    m_removeShortcut = new QShortcut(QKeySequence::Delete, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut->setEnabled(false);
    connect(m_removeShortcut, &QShortcut::activated, this, [=] { removeRow(currentIndex()); });

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut->setEnabled(true);
    connect(m_refreshShortcut, &QShortcut::activated, this, &TreeView::refresh);
}

void TreeView::contextMenuRequested(const QPoint &)
{
    BaseContextMenu menu(BaseContextMenu::Insert | BaseContextMenu::Edit | BaseContextMenu::Remove | BaseContextMenu::Refresh);

    emit(selectionModel()->currentChanged(currentIndex(), QModelIndex()));

    QAction *insertAction = menu.action(BaseContextMenu::Insert);
    insertAction->setShortcut(m_insertShortcut->key());
    insertAction->setEnabled(m_insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, [=] { insertRow(currentIndex()); });

    QAction *editAction = menu.action(BaseContextMenu::Edit);
    editAction->setShortcut(m_editShortcut->key());
    editAction->setEnabled(m_editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this, [=] { editRow(currentIndex()); });

    QAction *removeAction = menu.action(BaseContextMenu::Remove);
    removeAction->setShortcut(m_removeShortcut->key());
    removeAction->setEnabled(m_removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this, [=] { removeRow(currentIndex());  });

    QAction *refreshAction = menu.action(BaseContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, &TreeView::refresh);

    contextMenu(menu);
    menu.exec(QCursor().pos());
}

void TreeView::editRow(const QModelIndex &index)
{
    edit(index);
}

void TreeView::setInsertEnabled(bool ok)
{
    m_insertShortcut->setEnabled(ok);
}

void TreeView::setEditEnabled(bool ok)
{
    m_editShortcut->setEnabled(ok);
}

void TreeView::setRemoveEnabled(bool ok)
{
    m_removeShortcut->setEnabled(ok);
}

void TreeView::setRefreshEnabled(bool ok)
{
    m_refreshShortcut->setEnabled(ok);
}
