#include "stringsort.h"

StringSort::StringSort(QObject *parent) : QObject(parent)
{

}

bool StringSort::lessThen(QString left, QString right)
{
    // is alphabetical
    bool l_alpha = left.contains(QRegExp("\\D+"));
    bool r_alpha = right.contains(QRegExp("\\D+"));

    if ((l_alpha && r_alpha) || (!l_alpha && !r_alpha)) {
        left.remove(QRegExp("\\D+"));
        right.remove(QRegExp("\\D+"));

        return left.toInt() < right.toInt();
    } else if (l_alpha && !r_alpha) {
        return true;
    } else if (!l_alpha && r_alpha) {
        return false;
    }

    return false;
}
