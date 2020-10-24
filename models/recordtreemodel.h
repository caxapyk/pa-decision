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
    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, QVariant> columnHeaders;
    RecordNode *rootNode;

    void recursivelyRemoveNodes(RecordNode *node=nullptr);
};

#endif // RECORDTREEMODEL_H
