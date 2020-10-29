#include "decisionmodel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

DecisionModel::DecisionModel()
{
    m_nodeList = new NodeList;
}

DecisionModel::~DecisionModel()
{
    clear();
    delete m_nodeList;
}

void DecisionModel::clear()
{
    beginResetModel();
    for(int i = 0; i< m_nodeList->size(); ++i) {
        delete m_nodeList->at(i);
    }

    m_nodeList->clear();
    m_nodeList->squeeze();

    endResetModel();
}

void DecisionModel::select()
{
    beginResetModel();

    clear();
    setupModelData();

    endResetModel();
}

void DecisionModel::setupModelData()
{
    QSqlQuery query("SELECT id, name FROM pad_authority ORDER BY name ASC");

    while (query.next()) {
        Node *node = new Node;

        node->append(query.value(query.record().indexOf("id")));
        node->append(query.value(query.record().indexOf("name")));

        m_nodeList->append(node);
    }
}


int DecisionModel::columnCount(const QModelIndex &) const
{
    return 0;
}

QVariant DecisionModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            /*if(index.parent().isValid()) {
                const AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());
                return currentNode->at(1);
            }

            if(index.column() == 0) {
                return QVariant(tr("All Public Authorities"));
            }*/
        break;
        case Qt::EditRole:
            return data(index, Qt::DisplayRole);
        break;
        case Qt::DecorationRole:
            /*if (index.column() == 0) {
                if(index == rootItem())
                    return QIcon(":/icons/icons/folder-16.png");

                return QIcon(":/icons/icons/icon-16.png");
            }*/
        break;
        // return id of item
        case Qt::UserRole:
            /*if (index.column() == 0) {
                const AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());
                return currentNode->at(0);
            }*/
        break;
    }

    return QVariant();
}

QModelIndex DecisionModel::index(int row, int column, const QModelIndex &parent) const
{
    /*if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_rootNode);
    }

    return createIndex(row, column, m_nodeList->at(row));*/

    return QModelIndex();
}

QVariant DecisionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((section < 0)
            || ((orientation == Qt::Horizontal) && (section >= columnCount()))
            || ((orientation == Qt::Vertical) && (section >= rowCount()))) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return columnHeaders.value(section);
     }

    return QVariant();
}

QModelIndex DecisionModel::parent(const QModelIndex &index) const
{
    /*if (!index.isValid()) {
        return QModelIndex();
    }

    AT_Node *node = static_cast<AT_Node*>(index.internalPointer());
    if(node->isEmpty()) {
        return QModelIndex();
    }

    return createIndex(0, 0, m_rootNode);*/

    return QModelIndex();
}

int DecisionModel::rowCount(const QModelIndex &parent) const
{
    /*if (!parent.isValid()) {
        return 1;
    }

    AT_Node *node = static_cast<AT_Node*>(parent.internalPointer());
    if(node->isEmpty()) {
        return m_nodeList->size();
    }*/

    return 0;
}

bool DecisionModel::setData(const QModelIndex &index, const QVariant &value, int role){
    switch (role) {
        case Qt::EditRole:
            /*if(value.toString().size() > 0) {
                AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());

                QSqlQuery query;
                query.prepare("UPDATE pad_authority SET name=? WHERE id=?");
                query.bindValue(0, value);
                query.bindValue(1, currentNode->at(0));
                query.exec();

                if(query.isActive()) {
                    currentNode->replace(1, value);
                    emit dataChanged(index, index);

                    return true;
                }

                qDebug() << query.lastError().text();
            }*/
        break;
    }

    return false;
}

bool DecisionModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
{
    if ((section < 0)
            || ((orientation == Qt::Horizontal) && (section >= columnCount()))
            || ((orientation == Qt::Vertical) && (section >= rowCount()))) {
            return false;
    }

    if (orientation == Qt::Horizontal) {
        columnHeaders.insert(section, value);
        emit headerDataChanged(orientation, section, section);
        return true;
     }

     return false;
}
