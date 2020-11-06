#ifndef AUTHORITYFLATMODEL_H
#define AUTHORITYFLATMODEL_H

#include "authoritymodel.h"

class AuthorityFlatModel : public AuthorityModel
{
    Q_OBJECT
public:
    AuthorityFlatModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // AUTHORITYFLATMODEL_H
