#include "itemcounter.h"

ItemCounter::ItemCounter(QAbstractItemModel *model, QObject *parent) : QObject(parent),
    m_model(model)
{

}

int ItemCounter::next(int column, const QRegExp &rule, const QModelIndex &parent) const
{
    if(m_model) {
        int max = 1;
        for(int i = 0; i < m_model->rowCount(parent); ++i) {
            int num = 0;
            QModelIndex v = m_model->index(i, column, parent);

            if (rule.isEmpty() || v.data().toString().contains(rule)) {
                num = v.data().toString().remove(QRegExp("\\D+")).toInt();
            }

            if (num >= max) {
                max = num + 1;
            }
        }

        return max;
    }

    return -1;
}
