#include "subjectmodel.h"

#include <QDebug>
#include <QSqlRecord>

SubjectModel::SubjectModel(QObject *parent) :
    StandardReferenceModel(parent)
{
    setTable("pad_subject");

    setHeaderData(1, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));

    connect(sourceModel(), &QSqlTableModel::primeInsert, this, &SubjectModel::setDefaults);
}

void SubjectModel::setDefaults(int, QSqlRecord &record)
{
    record.setValue("name", tr("Subject type %1").arg(itemMaxNum(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}
