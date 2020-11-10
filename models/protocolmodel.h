#ifndef PROTOCOLMODEL_H
#define PROTOCOLMODEL_H

#include "referencemodel.h"

class ProtocolModel : public ReferenceModel
{
    Q_OBJECT
public:
    ProtocolModel(QObject *parent = nullptr);
    ~ProtocolModel();

    typedef QVector<QVariant> Node;
    typedef QVector<Node*> NodeList;

    void clear() override;
    void select() override;

    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    bool setRow(int row, QMap<QString, QVariant> &values, const QModelIndex &parent=QModelIndex()) override;

private:
    QMap<int, QVariant> columnHeaders;
    NodeList *m_nodeList;

    QMap<QString, QVariant> mSetRow;

    void setupModelData();
};

#endif // PROTOCOLMODEL_H
