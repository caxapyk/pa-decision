#include "recordflatmodel.h"

#include <QColor>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

RecordFlatModel::RecordFlatModel(QObject *parent)
    : SqlBaseModel(parent)
{
    m_internalModel = new QSqlQueryModel;
}

RecordFlatModel::~RecordFlatModel()
{
    clear();
    delete m_internalModel;
}

void RecordFlatModel::clear()
{
    beginResetModel();
    m_internalModel->clear();
    endResetModel();
}

void RecordFlatModel::select()
{
    beginResetModel();

    QSqlQuery query;
    query.prepare(tr(" \
                     SELECT \
                     pad_record.id, \
                     CONCAT(\"F.\", pad_fund.number, \" Inv.\", pad_inventory.number, \" R.\", pad_record.number) AS protocol \
                     FROM pad_record \
                     JOIN pad_inventory ON pad_record.inventory_id=pad_inventory.id \
                     JOIN pad_fund ON pad_inventory.fund_id=pad_fund.id \
                     %1 ORDER BY CAST(pad_fund.number AS UNSIGNED), CAST(pad_inventory.number AS UNSIGNED), CAST(pad_record.number AS UNSIGNED)"
                     ).arg(filter().isEmpty() ? QString() : filter()));

    if (query.exec()) {
        m_internalModel->setQuery(query);
    } else {
        qDebug () << query.lastError().text();
    }

    endResetModel();

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Protocol"));
}

QVariant RecordFlatModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_internalModel->headerData(section, orientation, role);
}

QModelIndex RecordFlatModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, nullptr);
    }

    return QModelIndex();
}

QModelIndex RecordFlatModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int RecordFlatModel::rowCount(const QModelIndex &parent) const
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->rowCount(internalParent);
}

int RecordFlatModel::columnCount(const QModelIndex &parent) const
{
    QModelIndex internalIndex = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->columnCount(internalIndex);
}

QVariant RecordFlatModel::data(const QModelIndex &index, int role) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->data(internalIndex, role);
}

bool RecordFlatModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return m_internalModel->setHeaderData(section, orientation, value, role);
}
