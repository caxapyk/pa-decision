#ifndef AUTHORITYPROXYMODEL_H
#define AUTHORITYPROXYMODEL_H

#include <QSortFilterProxyModel>

class AuthorityProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    AuthorityProxyModel();

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // AUTHORITYPROXYMODEL_H
