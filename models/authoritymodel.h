#ifndef AUTHORITYMODEL_H
#define AUTHORITYMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QRegExp>
#include <QVariant>
#include <QVector>

class AuthorityModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    typedef QVector<QVariant> AT_Node;
    typedef QVector<AT_Node*> AT_NodeList;

    AuthorityModel(QObject *parent = nullptr);
    ~AuthorityModel();

    int itemMaxNum(int column, const QRegExp &rule) const;
    QModelIndex rootItem() const;

    void clear();
    void select();

    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, QVariant> columnHeaders;

    void setupModelData();

protected:
    AT_Node *m_rootNode;
    AT_NodeList *m_nodeList;
};

#endif // AUTHORITYMODEL_H
