#ifndef AUTHORITYMODEL_H
#define AUTHORITYMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QRegExp>
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
    int itemMaxNum(int column, const QRegExp &rule) const;
    void select();
    void setupModelData();

    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex rootItem() const;
    int rowCount(const QModelIndex &parent) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    AT_Node *m_rootNode;
    AT_NodeList *m_nodeList;
};

#endif // AUTHORITYMODEL_H
