#include "customtreeview.h"

#include <QDebug>

CustomTreeView::CustomTreeView(QWidget *parent) : QTreeView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTreeView::EditKeyPressed);
    setSortingEnabled(true);

    setupShortcuts();

    connect(this, &QMenu::customContextMenuRequested, this, &CustomTreeView::contextMenuRequested);
}

CustomTreeView::~CustomTreeView()
{
    delete m_insertShortcut;
    delete m_editShortcut;
    delete m_removeShortcut;
    delete m_refreshShortcut;
}

void CustomTreeView::setupShortcuts()
{
    m_insertShortcut = new QShortcut(QKeySequence::New, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_insertShortcut->setEnabled(true);
    connect(m_insertShortcut, &QShortcut::activated, this, [=] { emit onInsert(currentIndex()); });

    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut->setEnabled(false);
    connect(m_editShortcut, &QShortcut::activated, this, [=] { emit onEdit(currentIndex()); });

    m_removeShortcut = new QShortcut(QKeySequence::Delete, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut->setEnabled(false);
    connect(m_removeShortcut, &QShortcut::activated, this, [=] { emit onRemove(currentIndex()); });

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut->setEnabled(true);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=]  { emit onRefresh(); });
}

void CustomTreeView::contextMenuRequested(const QPoint &)
{
    CustomContextMenu menu(CustomContextMenu::Insert | CustomContextMenu::Edit | CustomContextMenu::Remove | CustomContextMenu::Refresh);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(m_insertShortcut->key());
    insertAction->setEnabled(m_insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, [=] { emit onInsert(currentIndex()); });

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(m_editShortcut->key());
    editAction->setEnabled(m_editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this, [=] { emit onEdit(currentIndex()); });

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(m_removeShortcut->key());
    removeAction->setEnabled(m_removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this, [=] { emit onRemove(currentIndex()); });

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=]  { emit onRefresh(); });

    contextMenu(menu);
qDebug()<<"last";
    menu.exec(QCursor().pos());


}

void CustomTreeView::setInsertEnabled(bool ok)
{
    m_insertShortcut->setEnabled(ok);
}

void CustomTreeView::setEditEnabled(bool ok)
{
    m_editShortcut->setEnabled(ok);
}

void CustomTreeView::setRemoveEnabled(bool ok)
{
    m_removeShortcut->setEnabled(ok);
}

void CustomTreeView::setRefreshEnabled(bool ok)
{
    m_refreshShortcut->setEnabled(ok);
}
