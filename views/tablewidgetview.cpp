#include "tablewidgetview.h"

TableWidgetView::TableWidgetView(QWidget *parent) : QTableWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTableWidget::NoEditTriggers);
    setSortingEnabled(true);

    setupShortcuts();

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
    connect(m_insertShortcut, &QShortcut::activated, this, [=] { insertRow(currentRow()); });

    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut->setEnabled(false);
    connect(m_editShortcut, &QShortcut::activated, this, [=] { editRow(currentRow()); });

    m_removeShortcut = new QShortcut(QKeySequence::Delete, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut->setEnabled(false);
    connect(m_removeShortcut, &QShortcut::activated, this, [=] { removeRow(currentRow()); });

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut->setEnabled(true);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=] { refresh(); });
}

void TableWidgetView::contextMenuRequested(const QPoint &)
{
    BaseContextMenu menu(BaseContextMenu::Insert | BaseContextMenu::Edit | BaseContextMenu::Remove | BaseContextMenu::Refresh);

    QAction *insertAction = menu.action(BaseContextMenu::Insert);
    insertAction->setShortcut(m_insertShortcut->key());
    insertAction->setEnabled(m_insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, [=] { insertRow(0); });

    QAction *editAction = menu.action(BaseContextMenu::Edit);
    editAction->setShortcut(m_editShortcut->key());
    editAction->setEnabled(m_editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this, [=] { editRow(currentRow()); });

    QAction *removeAction = menu.action(BaseContextMenu::Remove);
    removeAction->setShortcut(m_removeShortcut->key());
    removeAction->setEnabled(m_removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this, [=] {
        removeRow(currentRow());
        removeRows(selectedRanges());
    });

    QAction *refreshAction = menu.action(BaseContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=] { refresh(); });

    contextMenu(menu);
    menu.exec(QCursor().pos());
}

void TableWidgetView::clear()
{
    clearContents();
    setRowCount(0);
}

void TableWidgetView::currentChanged(const QModelIndex &, const QModelIndex &)
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();

    if(ranges.length() == 0)
        setCurrentIndex(QModelIndex());

    setEditEnabled(ranges.length() > 0);
    setRemoveEnabled(ranges.length() > 0);
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
