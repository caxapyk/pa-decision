#include "documenttable.h"

#include <QDebug>
#include <QHeaderView>
#include <QSizePolicy>

DocumentTable::DocumentTable()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setSelectionBehavior(Table::SelectRows);
    setSelectionMode(Table::ExtendedSelection);

    setSortingEnabled(false);
    horizontalHeader()->setSortIndicatorShown(true);
    setColumnCount(m_headerLabels.length());

    setHorizontalHeaderLabels(m_headerLabels);
    hideColumn(0); //id
    horizontalHeader()->setSectionResizeMode(m_headerLabels.length() - 1, QHeaderView::Stretch);
}

void DocumentTable::contextMenu(BaseContextMenu &menu)
{
    menu.addAction("Open");
}

void DocumentTable::clear()
{
    clearContents();
    setRowCount(0);
}
