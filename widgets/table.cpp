#include "table.h"

#include <QDebug>

Table::Table(QWidget *parent) : QTableWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTableWidget::NoEditTriggers);
    setSortingEnabled(true);

    setupShortcuts();

    connect(this, &QMenu::customContextMenuRequested, this, &Table::contextMenuRequested);
    connect(this, &QTableWidget::itemSelectionChanged, this, &Table::onSelectionChanged);
}

Table::~Table()
{
    delete m_insertShortcut;
    delete m_editShortcut;
    delete m_removeShortcut;
    delete m_refreshShortcut;
}

void Table::setupShortcuts()
{
    m_insertShortcut = new QShortcut(QKeySequence::New, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_insertShortcut->setEnabled(true);
    connect(m_insertShortcut, &QShortcut::activated, this, [=] { emit onInsert(); });

    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut->setEnabled(false);
    connect(m_editShortcut, &QShortcut::activated, this, [=] { emit onEdit(currentRow()); });

    m_removeShortcut = new QShortcut(QKeySequence::Delete, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut->setEnabled(false);
    connect(m_removeShortcut, &QShortcut::activated, this, [=] { emit onRemove(selectedRanges()); });

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut->setEnabled(true);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=]  { emit onRefresh(Qt::AscendingOrder); });
}

void Table::contextMenuRequested(const QPoint &)
{
    BaseContextMenu menu(BaseContextMenu::Insert | BaseContextMenu::Edit | BaseContextMenu::Remove | BaseContextMenu::Refresh);

    QAction *insertAction = menu.action(BaseContextMenu::Insert);
    insertAction->setShortcut(m_insertShortcut->key());
    insertAction->setEnabled(m_insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, [=] { emit onInsert(); });

    QAction *editAction = menu.action(BaseContextMenu::Edit);
    editAction->setShortcut(m_editShortcut->key());
    editAction->setEnabled(m_editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this, [=] { emit onEdit(currentRow()); });

    QAction *removeAction = menu.action(BaseContextMenu::Remove);
    removeAction->setShortcut(m_removeShortcut->key());
    removeAction->setEnabled(m_removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this, [=] { emit onRemove(selectedRanges()); });

    QAction *refreshAction = menu.action(BaseContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=]  { emit onRefresh(Qt::AscendingOrder); });

    contextMenu(menu);

    menu.exec(QCursor().pos());
}

void Table::onSelectionChanged()
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();

    if(ranges.length() == 0)
        setCurrentIndex(QModelIndex());

    setEditEnabled(ranges.length() > 0);
    setRemoveEnabled(ranges.length() > 0);
}

void Table::setInsertEnabled(bool ok)
{
    m_insertEnabled = ok;
    if(isShortcutsEnabled())
        m_insertShortcut->setEnabled(ok);
}

void Table::setEditEnabled(bool ok)
{
    m_editEnabled = ok;
    if(isShortcutsEnabled())
        m_editShortcut->setEnabled(ok);
}

void Table::setRemoveEnabled(bool ok)
{
    m_removeEnabled = ok;
    if(isShortcutsEnabled())
        m_removeShortcut->setEnabled(ok);
}

void Table::setRefreshEnabled(bool ok)
{
    m_refreshEnabled = ok;
    if(isShortcutsEnabled())
        m_refreshShortcut->setEnabled(ok);
}

void Table::setContextMenuEnabled(bool ok)
{
    m_contextMenuEnabled = ok;
    if(!ok)
        disconnect(this, &QMenu::customContextMenuRequested, this, &Table::contextMenuRequested);
}

void Table::setShortcutsEnabled(bool ok)
{
    m_contextMenuEnabled = ok;
    if(!ok) {
        m_insertShortcut->setEnabled(false);
        m_editShortcut->setEnabled(false);
        m_removeShortcut->setEnabled(false);
        m_refreshShortcut->setEnabled(false);
    }
}
