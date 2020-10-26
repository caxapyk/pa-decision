#include "authorityproxymodel.h"
#include "utils/stringsort.h"

AuthorityProxyModel::AuthorityProxyModel()
{

}

bool AuthorityProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QString left = source_left.data().toString();
    QString right = source_right.data().toString();

    return StringSort::lessThen(left, right);
}
