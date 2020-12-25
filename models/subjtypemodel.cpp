#include "subjtypemodel.h"

#include <QDebug>
#include <QSqlRecord>

SubjtypeModel::SubjtypeModel(QObject *parent) :
    StandardReferenceModel(parent)
{
    setTable("pad_subjtype");

    setHeaderData(1, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));

    connect(sourceModel(), &QSqlTableModel::primeInsert, this, &SubjtypeModel::setDefaults);
}

void SubjtypeModel::setDefaults(int, QSqlRecord &record)
{
    record.setValue("name", tr("Subject type %1").arg(itemMaxNum(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}
