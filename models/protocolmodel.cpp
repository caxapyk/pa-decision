#include "protocolmodel.h"

#include <QDate>
#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

ProtocolModel::ProtocolModel(QObject *parent) : ReferenceModel(parent)
{
    m_internalModel = new QSqlTableModel;
    m_internalModel->setTable("pad_protocol");
    m_internalModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_internalModel->setSort(0, Qt::AscendingOrder);

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Authority ID"));
    setHeaderData(2, Qt::Horizontal, tr("Number"));
    setHeaderData(3, Qt::Horizontal, tr("Date"));
    setHeaderData(4, Qt::Horizontal, tr("Title"));
    setHeaderData(5, Qt::Horizontal, tr("Comment"));
}

ProtocolModel::~ProtocolModel()
{
    clear();
    delete m_internalModel;
}

void ProtocolModel::clear()
{
    beginResetModel();
    m_internalModel->clear();
    m_internalModel->setTable("pad_protocol");
    m_internalModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_internalModel->setSort(0, Qt::AscendingOrder);
    endResetModel();
}

void ProtocolModel::select()
{
    beginResetModel();
    if(authorityId()) {
        m_internalModel->setFilter("authority_id=" + QString::number(authorityId()));
        qDebug() << m_internalModel->filter();
    }
    m_internalModel->select();
    endResetModel();
}

bool ProtocolModel::submit()
{
    if(m_internalModel->submit()) {
        return true;
    }

    qDebug() << m_internalModel->lastError().text();

    return false;
}

Qt::ItemFlags ProtocolModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return ReferenceModel::flags(index);
}

QModelIndex ProtocolModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
            return QModelIndex();
        }

        if (!parent.isValid()) {
            return createIndex(row, column, nullptr);
        }

        return QModelIndex();
}

bool ProtocolModel::insertRows(int row, int count, const QModelIndex &parent)
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());

    beginInsertRows(parent, row, row + count -1);
    bool insert = m_internalModel->insertRows(row, count, internalParent);
    endInsertRows();

    if(insert && m_internalModel->submit()) {
        return true;
    }

    qDebug() << m_internalModel->lastError().text();

    return false;
}

QVariant ProtocolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_internalModel->headerData(section, orientation, role);
}

QModelIndex ProtocolModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool ProtocolModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());

    beginRemoveRows(parent, row, row + count -1);
    bool remove = m_internalModel->removeRows(row, count, internalParent);
    endRemoveRows();

    if(remove) {
        select();
        return true;
    }

    qDebug() << m_internalModel->lastError().text();

    return false;
}

int ProtocolModel::rowCount(const QModelIndex &parent) const
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->rowCount(internalParent);
}

int ProtocolModel::columnCount(const QModelIndex &index) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->columnCount(internalIndex);
}

QVariant ProtocolModel::data(const QModelIndex &index, int role) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->data(internalIndex, role);
}

bool ProtocolModel::setData(const QModelIndex &index, const QVariant &value, int role){
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());

    if(m_internalModel->setData(internalIndex, value, role)) {
        return true;
    }

    qDebug() << m_internalModel->lastError().text();

    return false;
}

bool ProtocolModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return m_internalModel->setHeaderData(section, orientation, value, role);
}
