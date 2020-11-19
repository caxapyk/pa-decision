#include "protocolflatmodel.h"

#include <QColor>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

ProtocolFlatModel::ProtocolFlatModel(QObject *parent)
    : ReferenceModel(parent)
{
    m_internalModel = new QSqlQueryModel;
}

ProtocolFlatModel::~ProtocolFlatModel()
{
    clear();
    delete m_internalModel;
}

void ProtocolFlatModel::clear()
{
    beginResetModel();
    m_internalModel->clear();
    endResetModel();
}

void ProtocolFlatModel::select()
{
    beginResetModel();

    if(authorityId()) {
        where("pad_protocol.authority_id=" + QString::number(authorityId()));
    }

    QSqlQuery query;
    query.prepare(tr(" \
                     SELECT \
                     pad_protocol.id, \
                     CONCAT(\"№\", number, \" from \", pad_protocol.date, \" \", pad_protocol.title) AS protocol \
                             FROM pad_protocol \
                             %1 ORDER BY pad_protocol.id DESC"
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

QVariant ProtocolFlatModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_internalModel->headerData(section, orientation, role);
}

QModelIndex ProtocolFlatModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, nullptr);
    }

    return QModelIndex();
}

QModelIndex ProtocolFlatModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int ProtocolFlatModel::rowCount(const QModelIndex &parent) const
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->rowCount(internalParent);
}

int ProtocolFlatModel::columnCount(const QModelIndex &parent) const
{
    QModelIndex internalIndex = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->columnCount(internalIndex);
}

QVariant ProtocolFlatModel::data(const QModelIndex &index, int role) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->data(internalIndex, role);
}

bool ProtocolFlatModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return m_internalModel->setHeaderData(section, orientation, value, role);
}

