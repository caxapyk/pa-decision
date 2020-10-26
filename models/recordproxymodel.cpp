#include "recordproxymodel.h"
#include "utils/stringsort.h"

RecordProxyModel::RecordProxyModel()
{

}

bool RecordProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QString left = source_left.data().toString();
    QString right = source_right.data().toString();

     return StringSort::lessThen(left, right);
}
