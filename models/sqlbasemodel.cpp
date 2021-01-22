#include "sqlbasemodel.h"

SqlBaseModel::SqlBaseModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void SqlBaseModel::where(const QString &condition)
{
    m_filter = QString(" WHERE %1").arg(condition);
}

void SqlBaseModel::andWhere(const QString &condition)
{
    m_filter = QString("%1 AND %2").arg(m_filter).arg(condition);
}

void SqlBaseModel::orWhere(const QString &condition)
{
    m_filter = QString("%1 OR %2").arg(m_filter).arg(condition);
}
