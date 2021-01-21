#include "standardreferencemodel.h"

#include <QDate>
#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

StandardReferenceModel::StandardReferenceModel(QObject *parent) : SqlBaseModel(parent)
{
    m_internalModel = new QSqlRelationalTableModel;
    m_internalModel->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    m_internalModel->setSort(0, Qt::AscendingOrder);
}

void StandardReferenceModel::setTable(const QString &table)
{
   m_internalModel->setTable(table);
}

StandardReferenceModel::~StandardReferenceModel()
{
    clear();
    delete m_internalModel;
}

void StandardReferenceModel::clear()
{
    beginResetModel();
    m_internalModel->clear();
    endResetModel();
}

void StandardReferenceModel::select()
{
    beginResetModel();
    if(authorityId()) {
        m_internalModel->setFilter("authority_id=" + QString::number(authorityId()));
    }
    bool s = m_internalModel->select();

    if(!s) {
        qDebug() << m_internalModel->lastError().text();
    }

    endResetModel();
}

bool StandardReferenceModel::submit()
{
    if(m_internalModel->submit()) {
        return true;
    }

    qDebug() << m_internalModel->lastError().text();
    select();

    return false;
}

Qt::ItemFlags StandardReferenceModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return SqlBaseModel::flags(index);
}

QModelIndex StandardReferenceModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
            return QModelIndex();
        }

        if (!parent.isValid()) {
            return createIndex(row, column, nullptr);
        }

        return QModelIndex();
}

bool StandardReferenceModel::insertRows(int row, int count, const QModelIndex &parent)
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());

    beginInsertRows(parent, row, row + count -1);
    bool insert = m_internalModel->insertRows(row, count, internalParent);
    endInsertRows();

    if(insert && m_internalModel->submit()) {
        return true;
    }

    qDebug() << m_internalModel->lastError().text();
    select();

    return false;
}

QVariant StandardReferenceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_internalModel->headerData(section, orientation, role);
}

QModelIndex StandardReferenceModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool StandardReferenceModel::removeRows(int row, int count, const QModelIndex &parent)
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
    select();

    return false;
}

int StandardReferenceModel::rowCount(const QModelIndex &parent) const
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->rowCount(internalParent);
}

int StandardReferenceModel::columnCount(const QModelIndex &parent) const
{
    QModelIndex internalIndex = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->columnCount(internalIndex);
}

QVariant StandardReferenceModel::data(const QModelIndex &index, int role) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->data(internalIndex, role);
}

bool StandardReferenceModel::setData(const QModelIndex &index, const QVariant &value, int role){
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());

    if(m_internalModel->setData(internalIndex, value, role)) {
        return true;
    }

    qDebug() << m_internalModel->lastError().text();
    select();

    return false;
}

bool StandardReferenceModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return m_internalModel->setHeaderData(section, orientation, value, role);
}

/// FIX IT
bool StandardReferenceModel::insertRecord(int row, const QSqlRecord &record)
{
    bool b = m_internalModel->insertRecord(row, record);
    if (b) {
        beginInsertRows(QModelIndex(), row, row);
        endInsertRows();
        emit dataChanged(index(0,0), index(0, columnCount() -1));
    }
    return b;
}
