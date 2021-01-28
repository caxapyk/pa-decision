#include "subjectview.h"

#include <QDebug>
#include <QHeaderView>

SubjectView::SubjectView(QWidget *parent) : TableWidgetView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);

    setColumnCount(m_subjectHeaderLabels.length());
    setHorizontalHeaderLabels(m_subjectHeaderLabels);
    horizontalHeader()->setSectionResizeMode(m_subjectHeaderLabels.length() - 1, QHeaderView::Stretch);
}

SubjectView::~SubjectView()
{

}

void SubjectView::contextMenu(BaseContextMenu &menu)
{
    menu.removeAction(menu.action(BaseContextMenu::Edit));
    menu.removeAction(menu.action(BaseContextMenu::Refresh));
}

void SubjectView::insertRow(int)
{
    int row = rowCount();

    setSortingEnabled(false);

    TableWidgetView::insertRow(row);

    QTableWidgetItem *action_item = new QTableWidgetItem("I");
    action_item->setFlags(Qt::NoItemFlags);
    setItem(row, 0, action_item);

    selectRow(row);
    setSortingEnabled(true);
}

void SubjectView::removeRow(int)
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
