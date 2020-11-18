#include "authorityflatmodel.h"

#include <QDebug>
#include <QIcon>

AuthorityFlatModel::AuthorityFlatModel(QObject *parent)
    :AuthorityModel(parent)
{

}

QModelIndex AuthorityFlatModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_nodeList->at(row));
    }

    return QModelIndex();
}

QModelIndex AuthorityFlatModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int AuthorityFlatModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
         return m_nodeList->size();
    }

    return 0;
}

int AuthorityFlatModel::columnCount(const QModelIndex &parent) const
{
    return AuthorityModel::columnCount(parent);
}

QVariant AuthorityFlatModel::data(const QModelIndex &index, int role) const
{
    const AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
    {
        return currentNode->at(index.column());
    }
        break;
    case Qt::DecorationRole:
        return QIcon(":/icons/icons/icon-16.png");
        break;
    }

    return QVariant();
}


