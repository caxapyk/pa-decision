#ifndef RECORDTREEMODEL_H
#define RECORDTREEMODEL_H

#include <QAbstractItemModel>

class RecordTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    RecordTreeModel();
    ~RecordTreeModel();

    struct RecordNode
    {
        QVariant id;
        QVariant number;
        QVector<RecordNode*> children;
        RecordNode* parent;
        int level;
        int row;
        bool mapped;
    };

    enum {FundLevel, InventoryLevel, RecordLevel};

    void clear();
    void select();
    void setupModelData(const QModelIndex &index=QModelIndex());

    bool canFetchMore(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;

private:
    RecordNode *rootNode;

    void recursivelyRemoveNodes(RecordNode *node=nullptr);
};

#endif // RECORDTREEMODEL_H
