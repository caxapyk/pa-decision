#ifndef COLLECTIONMODEL_H
#define COLLECTIONMODEL_H

#include <QAbstractItemModel>

class CollectionModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CollectionModel(QObject *parent = nullptr);

    virtual QVariant headerData(int, Qt::Orientation, int role = Qt::DisplayRole) const = 0;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const = 0;
    virtual QModelIndex parent(const QModelIndex &index) const = 0;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;

    virtual void clear() = 0;
    virtual void select() = 0;
};

#endif // COLLECTIONMODEL_H
