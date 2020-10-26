#include "recordproxymodel.h"

RecordProxyModel::RecordProxyModel()
{

}

bool RecordProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QString left = source_left.data().toString();
    QString right = source_right.data().toString();

    // is alphabetical
    bool l_alpha = left.contains(QRegExp("\\D+"));
    bool r_alpha = right.contains(QRegExp("\\D+"));

    if ((l_alpha && r_alpha) || (!l_alpha && !r_alpha)) {
        left.remove(QRegExp("\\D+"));
        right.remove(QRegExp("\\D+"));

        return left.toInt() > right.toInt();
    } else if (l_alpha && !r_alpha) {
        return true;
    } else if (!l_alpha && r_alpha) {
        return false;
    }

    return false;
}
