#include "subjecttable.h"

#include <QDebug>
#include <QHeaderView>

SubjectTable::SubjectTable(QWidget *parent) : Table(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);

    setColumnCount(m_subjectHeaderLabels.length());
    setHorizontalHeaderLabels(m_subjectHeaderLabels);
    horizontalHeader()->setSectionResizeMode(m_subjectHeaderLabels.length() - 1, QHeaderView::Stretch);

    connect(this, &Table::onInsert, this, &SubjectTable::insertRowPrivate);
    connect(this, &Table::onRemove, this, &SubjectTable::removeRowPrivate);
}

void SubjectTable::contextMenu(BaseContextMenu &menu)
{
    menu.removeAction(menu.action(BaseContextMenu::Edit));
    menu.removeAction(menu.action(BaseContextMenu::Refresh));
}

void SubjectTable::insertRowPrivate()
{
    int row = rowCount();

    setSortingEnabled(false);

    Table::insertRow(row);

    QTableWidgetItem *action_item = new QTableWidgetItem("I");
    action_item->setFlags(Qt::NoItemFlags);
    setItem(row, 0, action_item);

    selectRow(row);
    setSortingEnabled(true);
}

void SubjectTable::removeRowPrivate()
{
    int row = currentRow();

    QString action = item(row, 0)->text();

    if(action == "I") {
        removeRow(row);
    } else {
        item(currentRow(), 0)->setText("R");
        hideRow(row);
    }
}
