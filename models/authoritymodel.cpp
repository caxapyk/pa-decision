#include "authoritymodel.h"

#include <QDebug>

AuthorityModel::AuthorityModel()
{

}

QModelIndex AuthorityModel::index(int row, int column, const QModelIndex &parent) const
{

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        //return createIndex(row, column, const_cast<HierarchyNode*>(root->children[row]));
    }

    //HierarchyNode *parentNode = static_cast<HierarchyNode*>(parent.internalPointer());

    //return createIndex(row, column, parentNode->children[row]);

    return createIndex(row, column, new AuthorityModel());

}

QModelIndex AuthorityModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    //HierarchyNode *childNode = static_cast<HierarchyNode*>(index.internalPointer());
    //HierarchyNode *parentNode = childNode->parent;

    //if (parentNode != nullptr) {
        //return createIndex(parentNode->row, 0, parentNode);
    //}

    return QModelIndex();
}

int AuthorityModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        //return root->children.size();
         return 5;
    }

    //const HierarchyNode* parentNode = static_cast<const HierarchyNode*>(parent.internalPointer());
    //return parentNode->children.size();
    return 0;
}

int AuthorityModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant AuthorityModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            return QVariant("test" + QString::number(index.row()));
        break;
    }

    return QVariant();
}
