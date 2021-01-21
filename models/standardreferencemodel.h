#ifndef STANDARDREFERENCEMODEL_H
#define STANDARDREFERENCEMODEL_H

#include "sqlbasemodel.h"

#include <QSqlRelationalTableModel>
#include <QSqlRecord>

class StandardReferenceModel : public SqlBaseModel
{
    Q_OBJECT
public:
    StandardReferenceModel(QObject *parent = nullptr);
    ~StandardReferenceModel();

    void clear() override;
    void select() override;

    bool submit() override;

    QSqlRelationalTableModel *sourceModel() { return m_internalModel; };
    void setTable(const QString &table);


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

    bool insertRecord(int row, const QSqlRecord &record);

private:
    QSqlRelationalTableModel *m_internalModel;
    QString m_table;
};

#endif // STANDARDREFERENCEMODEL_H
