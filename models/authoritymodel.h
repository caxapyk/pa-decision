#ifndef AUTHORITYMODEL_H
#define AUTHORITYMODEL_H

#include <QAbstractItemModel>

class AuthorityModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    AuthorityModel();

    void setupModelData(const QModelIndex &index=QModelIndex());

    //bool canFetchMore(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //void fetchMore(const QModelIndex &parent) override;
    //bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // AUTHORITYMODEL_H
