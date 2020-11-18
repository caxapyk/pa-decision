#include "referencemodel.h"

ReferenceModel::ReferenceModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void ReferenceModel::where(const QString &condition)
{
    m_filter = QString(" WHERE %1").arg(condition);
}

void ReferenceModel::andWhere(const QString &condition)
{
    m_filter = QString("%1 AND %2").arg(m_filter).arg(condition);
}

void ReferenceModel::orWhere(const QString &condition)
{
    m_filter = QString("%1 OR %2").arg(m_filter).arg(condition);
}

int ReferenceModel::itemMaxNum(int column, const QRegExp &rule) const
{
    int max = 1;
    for(int i = 0; i < rowCount(); ++i) {
        int num = 0;
        QModelIndex v = index(i, column);

        if (rule.isEmpty() || v.data().toString().contains(rule)) {
            num = v.data().toString().remove(QRegExp("\\D+")).toInt();
        }

        if (num >= max) {
            max = num + 1;
        }
    }

    return max;
}
