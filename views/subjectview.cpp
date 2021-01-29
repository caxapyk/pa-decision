#include "subjectview.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

SubjectView::SubjectView(QWidget *parent) : TableWidgetView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
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

void SubjectView::_insertRow()
{
    setSortingEnabled(false);

    insertRow(0);

    QTableWidgetItem *action_item = new QTableWidgetItem("I");
    action_item->setFlags(Qt::NoItemFlags);
    setItem(0, 0, action_item);

    selectRow(0);
    setSortingEnabled(true);
}

void SubjectView::removeRows()
{
    const QList<QTableWidgetSelectionRange> range = selectedRanges();

    int res = QMessageBox::critical(this,
        tr("Documents"),
        tr("Are you shure that you want to delete %1 item(s)?").arg(range.length()),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        for (int i = 0; i < range.length(); ++i) {
            int row = range.at(i).topRow() - i;
            QString action = item(row, 0)->text();

            if(action == "I") {
                removeRow(row);
            } else {
                item(currentRow(), 0)->setText("R");
                hideRow(row);
            }
        }
    }
}
