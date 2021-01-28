#ifndef RECORDTREEMODEL_H
#define RECORDTREEMODEL_H

#include <QAbstractItemModel>

class AFTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    AFTreeModel(QObject *parent = nullptr);
    ~AFTreeModel();

    struct RecordNode
    {
        QVariant id;
        QVariant number;
        QVariant comment;
        QVariant name;
        QVector<RecordNode*> children;
        RecordNode* parent;
        int level;
        int row;
        bool mapped;
    };

    enum Levels {FundLevel, InventoryLevel, RecordLevel};

    QVariant authorityId() const { return m_authorityId; };
    void setAuthorityId(const QVariant &id) { m_authorityId = id; };

    QVariant fundId() const { return m_fundId; };
    void setFundId(const QVariant &id) { m_fundId = id; };

    QVariant inventoryId() const { return m_inventoryId; };
    void setInventoryId(const QVariant &id) { m_inventoryId = id; };

    void setMaxDepth(int depth) { m_maxDepth = depth; };

    void clear();
    void select();

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
    RecordNode *rootNode;

    QMap<int, QVariant> columnHeaders;

    QVariant m_authorityId;
    QVariant m_fundId;
    QVariant m_inventoryId;

    int m_maxDepth = 3;

    void setupModelData(const QModelIndex &index=QModelIndex());
    void recursivelyRemoveNodes(RecordNode *node=nullptr);
};

#endif // RECORDTREEMODEL_H
