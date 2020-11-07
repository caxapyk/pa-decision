#include "referencemodel.h"

ReferenceModel::ReferenceModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void ReferenceModel::where(const QString &condition)
{
    cond = QString(" WHERE %1").arg(condition);
}

void ReferenceModel::andWhere(const QString &condition)
{
    cond = QString("%1 AND %2").arg(cond).arg(condition);
}

void ReferenceModel::orWhere(const QString &condition)
{
    cond = QString("%1 OR %2").arg(cond).arg(condition);
}
