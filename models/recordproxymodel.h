#ifndef RECORDPROXYMODEL_H
#define RECORDPROXYMODEL_H

#include <QSortFilterProxyModel>

class RecordProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    RecordProxyModel();

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // RECORDPROXYMODEL_H
