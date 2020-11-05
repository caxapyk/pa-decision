#ifndef RECORDTREEMODEL_H
#define RECORDTREEMODEL_H

#include "models/referencemodel.h"

#include <QAbstractItemModel>

class RecordModel : public ReferenceModel
{
    Q_OBJECT

public:
    RecordModel(QObject *parent = nullptr);
    ~RecordModel();

    struct RecordNode
    {
        QVariant id;
        QVariant number;
        QVariant comment;
        QVector<RecordNode*> children;
        RecordNode* parent;
        int level;
        int row;
        bool mapped;
    };

    enum Levels {FundLevel, InventoryLevel, RecordLevel};

    void clear() override;
    void select() override;

    bool canFetchMore(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, QVariant> columnHeaders;
    QMap<RecordNode*, QVariant> fundNames;

    RecordNode *rootNode;

    void setupModelData(const QModelIndex &index=QModelIndex());
    void recursivelyRemoveNodes(RecordNode *node=nullptr);
};

#endif // RECORDTREEMODEL_H
