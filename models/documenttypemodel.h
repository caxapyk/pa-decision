#ifndef DOCUMENTTYPEMODEL_H
#define DOCUMENTTYPEMODEL_H

#include "models/referencemodel.h"

#include <QSqlTableModel>

class DocumentTypeModel : public ReferenceModel
{

    Q_OBJECT
public:
    DocumentTypeModel();
    ~DocumentTypeModel();

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
    QSqlTableModel *m_internalModel;

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // DOCUMENTTYPEMODEL_H
