#ifndef DOCUMENTTYPEMODEL_H
#define DOCUMENTTYPEMODEL_H

#include "models/referencemodel.h"

#include <QAbstractItemModel>

class DocumentTypeModel : public ReferenceModel
{

    Q_OBJECT

public:
    typedef QVector<QVariant> Node;
    typedef QVector<Node*> NodeList;

    DocumentTypeModel();
    ~DocumentTypeModel();

    int itemMaxNum(int column, const QRegExp &rule) const;

    void clear() override;
    void select() override;

    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
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
    NodeList *m_nodeList;
};

#endif // DOCUMENTTYPEMODEL_H
