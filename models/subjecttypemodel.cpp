#include "subjecttypemodel.h"

#include "utils/itemcounter.h"

#include <QDebug>
#include <QSqlRecord>

SubjectTypeModel::SubjectTypeModel(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable("pad_subjtype");
    setEditStrategy(QSqlTableModel::OnFieldChange);

    setHeaderData(1, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));

    connect(this, &QSqlTableModel::primeInsert, this, &SubjectTypeModel::setDefaults);
}

Qt::ItemFlags SubjectTypeModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void SubjectTypeModel::setDefaults(int, QSqlRecord &record)
{
    ItemCounter counter(this);

    record.setValue("name", tr("Subject type %1").arg(counter.next(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}


