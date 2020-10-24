#include "recordtreemodel.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

RecordTreeModel::RecordTreeModel()
{
    rootNode = new RecordNode;
    setHeaderData(0, Qt::Horizontal, tr("Archive records"));
}

RecordTreeModel::~RecordTreeModel()
{
    clear();
    delete rootNode;
}

void RecordTreeModel::clear()
{
    beginResetModel();

    for(int i = 0; i < rootNode->children.length(); ++i) {
        recursivelyRemoveNodes(rootNode->children.at(i));
    }

    rootNode->children.clear();
    rootNode->children.squeeze();

    endResetModel();
}

/* Removes all nodes objects from memory recursively */
void RecordTreeModel::recursivelyRemoveNodes(RecordNode *node)
{
    for(int i = 0; i < node->children.length(); ++i) {
        recursivelyRemoveNodes(node->children.at(i));
    }

    delete node;
}

void RecordTreeModel::select()
{
    clear();
    setupModelData();
}

void RecordTreeModel::setupModelData(const QModelIndex &index)
{
    RecordNode *parentNode = (index.isValid()) ? static_cast<RecordNode*>(index.internalPointer()) : rootNode;
    int level = (index.isValid()) ? parentNode->level + 1 : RecordTreeModel::FundLevel;

    QSqlQuery query;

    switch (level) {
    case RecordTreeModel::FundLevel:
        query.prepare("SELECT id, number FROM pad_fund ORDER BY CAST(number AS UNSIGNED) DESC");
        break;
    case RecordTreeModel::InventoryLevel:
        query.prepare("SELECT id, number FROM pad_inventory WHERE fund_id=? ORDER BY CAST(number AS UNSIGNED) DESC");
        query.bindValue(0, parentNode->id.toInt());
        break;
    case RecordTreeModel::RecordLevel:
        query.prepare("SELECT id, number FROM pad_record WHERE inventory_id=? ORDER BY CAST(number AS UNSIGNED) DESC");
        query.bindValue(0, parentNode->id.toInt());
        break;
    }

    query.exec();

    if(query.isActive()) {
        while(query.next()) {
            RecordNode *node = new RecordNode();

            node->id = query.record().value(0);
            node->number = query.record().value(1);

            node->level = level;
            node->row = query.at();
            node->mapped = false;
            node->parent = (level != FundLevel) ? parentNode : nullptr;

            parentNode->mapped = (level != FundLevel);
            parentNode->children.append(node);
        }
    } else {
        qDebug() << query.lastError().text();
        return;
    }
}

bool RecordTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return !rootNode->children.isEmpty();
    }

    RecordNode *parentNode = static_cast<RecordNode*>(parent.internalPointer());

    QSqlQuery query;

    switch (parentNode->level) {
    case RecordTreeModel::FundLevel:
        query.prepare("SELECT COUNT(id) FROM pad_inventory WHERE fund_id=?");
        query.bindValue(0, parentNode->id.toInt());
        break;
    case RecordTreeModel::InventoryLevel:
        query.prepare("SELECT COUNT(id) FROM pad_record WHERE inventory_id=?");
        query.bindValue(0, parentNode->id.toInt());
        break;
    case RecordTreeModel::RecordLevel:
        return false;
        break;
    default:
        return false;
        break;
    }

    query.exec();

    query.next();
    if (query.size() > 0 && query.value(0).toInt() > 0) {
        return true;
    }

    return false;
}

bool RecordTreeModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return false;
    }

    const RecordNode *parentNode = static_cast<const RecordNode*>(parent.internalPointer());

    return !parentNode->mapped;
}

void RecordTreeModel::fetchMore(const QModelIndex &parent)
{
    setupModelData(parent);
}

QVariant RecordTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex RecordTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
    return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, const_cast<RecordNode*>(rootNode->children[row]));
    }

    RecordNode *parentNode = static_cast<RecordNode*>(parent.internalPointer());

    return createIndex(row, column, parentNode->children[row]);
}

QModelIndex RecordTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    RecordNode *childNode = static_cast<RecordNode*>(index.internalPointer());
    RecordNode *parentNode = childNode->parent;

    if (parentNode != nullptr) {
        return createIndex(parentNode->row, 0, parentNode);
    }

    return QModelIndex();
}

int RecordTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return rootNode->children.size();
    }

    const RecordNode* parentNode = static_cast<const RecordNode*>(parent.internalPointer());

    return parentNode->children.size();
}

int RecordTreeModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant RecordTreeModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
        {
            const RecordNode* currentNode = static_cast<RecordNode*>(index.internalPointer());
            return currentNode->number;
        }
            break;
        case Qt::EditRole:
            return data(index, Qt::DisplayRole);
            break;
        case Qt::DecorationRole:
            break;
        // return id of item
        case Qt::UserRole:
            break;
    }

    return QVariant();
}

bool RecordTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
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
