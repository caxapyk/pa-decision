#ifndef DECISIONMODEL_H
#define DECISIONMODEL_H

#include <QAbstractItemModel>

class DecisionModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    typedef QVector<QVariant> Node;
    typedef QVector<Node*> NodeList;

    DecisionModel();
    ~DecisionModel();

    void clear();
    void select();
    void setupModelData();

    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, QVariant> columnHeaders;
    NodeList *m_nodeList;

};

#endif // DECISIONMODEL_H
