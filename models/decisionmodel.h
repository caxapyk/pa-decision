#ifndef DECISIONMODEL_H
#define DECISIONMODEL_H

#include <QAbstractItemModel>

class DecisionModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Column{
        Date,
        Number,
        Title,
        ARecord,
        Comment,
        ColumnCount
    };

    typedef QMap<QString, QVariant> Node;
    typedef QVector<Node*> NodeList;

    DecisionModel();
    ~DecisionModel();

    void clear();
    void select();
    void setupModelData();

    void andWhere(const QString &condition);
    void orWhere(const QString &condition);
    void where(const QString &condition);

    QString field(const QModelIndex &index) const;

    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, QVariant> columnHeaders;
    NodeList *m_nodeList;

    QString cond;
};

#endif // DECISIONMODEL_H
