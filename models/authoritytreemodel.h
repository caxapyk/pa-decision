#ifndef AUTHORITYMODEL_H
#define AUTHORITYMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QVariant>
#include <QVector>

class AuthorityTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    typedef QVector<QVariant> AT_Node;
    typedef QVector<AT_Node*> AT_NodeList;

    AuthorityTreeModel();
    ~AuthorityTreeModel();
    void clear();
    void select();
    void setupModelData();

    //bool canFetchMore(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    //void fetchMore(const QModelIndex &parent) override;
    //bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    int m_cols = 1; // columns offset (need for hide id column)
    int m_offset = 1; // columns offset (need for hide id column)
    AT_Node *m_rootNode;
    AT_NodeList *m_nodeList;
};

#endif // AUTHORITYMODEL_H
