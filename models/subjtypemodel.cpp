#include "subjtypemodel.h"

#include "utils/itemcounter.h"

#include <QDebug>
#include <QSqlRecord>

SubjtypeModel::SubjtypeModel(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable("pad_subjtype");
    setEditStrategy(QSqlTableModel::OnRowChange);

    setHeaderData(1, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));

    connect(this, &QSqlTableModel::primeInsert, this, &SubjtypeModel::setDefaults);
}

void SubjtypeModel::setDefaults(int, QSqlRecord &record)
{
    ItemCounter counter(this);

    record.setValue("name", tr("Subject type %1").arg(counter.next(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}


