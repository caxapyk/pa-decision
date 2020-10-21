#include "authoritytreemodel.h"

#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

AuthorityTreeModel::AuthorityTreeModel()
{
    m_rootNode = new AT_Node;
    m_nodeList = new AT_NodeList;
}

AuthorityTreeModel::~AuthorityTreeModel()
{
    clear();
    delete m_rootNode;
    delete m_nodeList;
}

void AuthorityTreeModel::clear()
{
    beginResetModel();
    for(int i = 0; i< m_nodeList->size(); ++i) {
        delete m_nodeList->at(i);
    }

    m_nodeList->clear();
    m_nodeList->squeeze();

    endResetModel();
}

void AuthorityTreeModel::setupModelData()
{
    QSqlQuery query("SELECT id, name FROM authority");

    m_cols = query.record().count();

    while (query.next()) {
        AT_Node *node = new AT_Node;

        node->append(query.value(query.record().indexOf("id")));
        node->append(query.value(query.record().indexOf("name")));

        m_nodeList->append(node);
    }
}

void AuthorityTreeModel::select()
{
    clear();
    setupModelData();
}

Qt::ItemFlags AuthorityTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QAbstractItemModel::flags(index);
    }

    AT_Node *node = static_cast<AT_Node*>(index.internalPointer());
    if(node->isEmpty()) {
        return QAbstractItemModel::flags(index);
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QModelIndex AuthorityTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_rootNode);
    }

    return createIndex(row, column, m_nodeList->at(row));
}

QModelIndex AuthorityTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    AT_Node *node = static_cast<AT_Node*>(index.internalPointer());
    if(node->isEmpty()) {
        return QModelIndex();
    }

    return createIndex(0, 0, m_rootNode);
}

int AuthorityTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }

    AT_Node *node = static_cast<AT_Node*>(parent.internalPointer());
    if(node->isEmpty()) {
        return m_nodeList->size();
    }

    return 0;

}

int AuthorityTreeModel::columnCount(const QModelIndex &) const
{
    return m_cols - m_offset;
}

QVariant AuthorityTreeModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            if(index.parent().isValid()) {
                const AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());
                return currentNode->at(index.column() + m_offset);
            }

            if(index.column() == 0) {
                return QVariant(tr("All Public Authorities"));
            }

        break;
        case Qt::EditRole:
            return data(index, Qt::DisplayRole);
        break;
        case Qt::DecorationRole:
            if (index.column() == 0) {
                return QIcon(":/icons/icons/folder-16.png");
            }
        break;
    }

    return QVariant();
}

bool AuthorityTreeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    switch (role) {
        case Qt::EditRole:
            if(value.toString().size() > 0) {
                AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());

                QSqlQuery query;
                query.prepare("UPDATE authority SET name=? WHERE id=?");
                query.bindValue(0, value);
                query.bindValue(1, currentNode->at(0));
                query.exec();

                if(query.isActive()) {
                    currentNode->replace(index.column() + m_offset, value);
                    emit dataChanged(index, index);

                    return true;
                }

                qDebug() << query.lastError().text();
            }
        break;
    }

    return false;
}
