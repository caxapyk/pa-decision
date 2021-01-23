#include "decisiontable.h"

#include <QDebug>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSizePolicy>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>


DecisionTable::DecisionTable()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setSelectionBehavior(Table::SelectRows);
    setSelectionMode(Table::ExtendedSelection);
}

void DecisionTable::refresh()
{
    if (!m_authorityId.isValid())
        return;

    QSqlQuery query;
    query.prepare("select id, number, name, comment from pad_decision where authority_id=?");
    query.bindValue(0, m_authorityId);
    query.exec();

    if(query.isActive()) {
        if(!query.size())
            return;

        int cols = query.record().count();
        setColumnCount(cols);
        setHorizontalHeaderLabels(m_headerLabels);

        while(query.next()) {
            setRowCount(rowCount() + 1);

            int row = query.at();
             for (int j = 0; j < cols; ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(j).toString());
                setItem(row, j, item);

                // stretch last item
                if(j == cols - 1)
                    horizontalHeader()->setSectionResizeMode(j, QHeaderView::Stretch);
             }
        }
    } else {
        qDebug() << query.lastError().text();
    }
}
