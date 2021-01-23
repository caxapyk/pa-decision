#include "recordmodel.h"

#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

RecordModel::RecordModel(QObject *parent) : QAbstractItemModel(parent)
{
    rootNode = new RecordNode;
    setHeaderData(0, Qt::Horizontal, tr("Archive records"));
    setHeaderData(1, Qt::Horizontal, tr("Comment"));
    setHeaderData(2, Qt::Horizontal, tr("ID"));
    setHeaderData(3, Qt::Horizontal, tr("Name"));
}

RecordModel::~RecordModel()
{
    clear();
    delete rootNode;
}

void RecordModel::clear()
{
    beginResetModel();

    for(int i = 0; i < rootNode->children.length(); ++i) {
        recursivelyRemoveNodes(rootNode->children.at(i));
    }

    rootNode->children.clear();
    rootNode->children.squeeze();

    //clearFilter();

    endResetModel();
}

/* Removes all nodes objects from memory recursively */
void RecordModel::recursivelyRemoveNodes(RecordNode *node)
{
    for(int i = 0; i < node->children.length(); ++i) {
        recursivelyRemoveNodes(node->children.at(i));
    }

    delete node;
}

void RecordModel::select()
{
    beginResetModel();

    clear();
    setupModelData();

    endResetModel();
}

void RecordModel::setupModelData(const QModelIndex &index)
{
    RecordNode *parentNode = (index.isValid()) ? static_cast<RecordNode*>(index.internalPointer()) : rootNode;
    int level = (index.isValid()) ? parentNode->level + 1 : RecordModel::FundLevel;

    QSqlQuery query;
    QString filter;

    switch (level) {
    case RecordModel::FundLevel:
    {
        if(m_authorityId.isValid())
            filter = "WHERE authority_id=" + authorityId().toString();

        if(m_fundId.isValid())
            filter = (filter.isEmpty() ? "WHERE " : (filter + " AND ")) + ("id=" + m_fundId.toString());

        qDebug() << filter;
        query.prepare(QString("SELECT number, comment, id, name FROM pad_fund %1 ORDER BY CAST(number AS UNSIGNED) ASC").arg(filter));
    }
        break;
    case RecordModel::InventoryLevel:
    {
        if(m_inventoryId.isValid())
            filter = (filter.isEmpty() ? "WHERE " : (filter + " AND ")) + ("id=" + m_inventoryId.toString());

        query.prepare("SELECT number, comment, id, name FROM pad_inventory WHERE fund_id=? ORDER BY CAST(number AS UNSIGNED) ASC");
        query.bindValue(0, parentNode->id.toInt());
    }
        break;
    case RecordModel::RecordLevel:
    {
        query.prepare("SELECT number, comment, id, name FROM pad_record WHERE inventory_id=? ORDER BY CAST(number AS UNSIGNED) ASC");
        query.bindValue(0, parentNode->id.toInt());
    }
        break;
    }

    query.exec();

    if(query.isActive()) {
        while(query.next()) {
            RecordNode *node = new RecordNode();

            node->id = query.record().value(2);
            node->number.setValue(query.record().value(0));
            node->comment.setValue(query.record().value(1).toString());
            node->name.setValue(query.record().value(3).toString());

            node->level = level;
            node->row = query.at();
            node->mapped = false;
            node->parent = (level != FundLevel) ? parentNode : nullptr;

            parentNode->mapped = (level != FundLevel);
            parentNode->children.append(node);
        }
    } else {
        qDebug() << query.lastError().text();
    }
}

Qt::ItemFlags RecordModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() != 0) {
        return QAbstractItemModel::flags(index);
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool RecordModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return !rootNode->children.isEmpty();
    }

    RecordNode *parentNode = static_cast<RecordNode*>(parent.internalPointer());

    QSqlQuery query;

    switch (parentNode->level) {
    case RecordModel::FundLevel:
        query.prepare("SELECT COUNT(id) FROM pad_inventory WHERE fund_id=?");
        query.bindValue(0, parentNode->id.toInt());
        break;
    case RecordModel::InventoryLevel:
        query.prepare("SELECT COUNT(id) FROM pad_record WHERE inventory_id=?");
        query.bindValue(0, parentNode->id.toInt());
        break;
    case RecordModel::RecordLevel:
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

bool RecordModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return false;
    }

    const RecordNode *parentNode = static_cast<const RecordNode*>(parent.internalPointer());

    return !parentNode->mapped;
}

void RecordModel::fetchMore(const QModelIndex &parent)
{
    setupModelData(parent);
}

QVariant RecordModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex RecordModel::index(int row, int column, const QModelIndex &parent) const
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

bool RecordModel::insertRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;
    QVariant name;

    RecordNode *parentNode = (parent.isValid()) ? static_cast<RecordNode*>(parent.internalPointer()) : rootNode;

    if(!parent.isValid()) {
        if(authorityId() == 0) {
            return false;
        }
        name = tr("New fund");
        query.prepare("INSERT INTO pad_fund(number, name, authority_id) VALUES (?,?,?)");
        query.bindValue(0, name);
        query.bindValue(1, name);
        query.bindValue(2, authorityId());
    } else if(parentNode->level == RecordModel::FundLevel) {
        name = tr("New inventory");
        query.prepare("INSERT INTO pad_inventory(number, name, fund_id) VALUES (?,?,?)");
        query.bindValue(0, name);
        query.bindValue(1, name);
        query.bindValue(2, parentNode->id);
    } else if(parentNode->level == RecordModel::InventoryLevel) {
        name = tr("New record");
        query.prepare("INSERT INTO pad_record(number, name, inventory_id) VALUES (?,?,?)");
        query.bindValue(0, name);
        query.bindValue(1, name);
        query.bindValue(2, parentNode->id);
    }

    query.exec();

    if(query.isActive()) {

        beginInsertRows(parent, row, row + count - 1);

        RecordNode *node = new RecordNode();

        node->id = QVariant(query.lastInsertId());
        node->number.setValue(name);

        node->level = parent.isValid() ? parentNode->level + 1 : RecordModel::FundLevel;
        node->row = row;
        node->mapped = false;
        node->parent = parent.isValid() ? parentNode : nullptr;

        parentNode->mapped = parent.isValid();
        parentNode->children.insert(row, node);

        endInsertRows();

        emit dataChanged(parent, parent);

        return true;
    }

    qDebug() << query.lastError().text();

    return false;
}

QModelIndex RecordModel::parent(const QModelIndex &index) const
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

bool RecordModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;

    for(int i = row; i < (row + count); ++i) {

        RecordNode *currentNode = static_cast<RecordNode*>(index(i, 0, parent).internalPointer());

        switch (currentNode->level) {
        case RecordModel::FundLevel:
            query.prepare("DELETE FROM pad_fund WHERE id=?");
            break;
        case RecordModel::InventoryLevel:
            query.prepare("DELETE FROM pad_inventory WHERE id=?");
            break;
        case RecordModel::RecordLevel:
            query.prepare("DELETE FROM pad_record WHERE id=?");
            break;
        }

        query.bindValue(0, currentNode->id);
        query.exec();

        if(query.isActive()) {
            RecordNode *parentNode = (parent.isValid()) ? static_cast<RecordNode*>(parent.internalPointer()) : rootNode;

            beginRemoveRows(parent, row, row);

            delete currentNode;

            parentNode->children.removeAt(row);

            endRemoveRows();

            return true;
        }

        qDebug() << query.lastError().text();
    }

    return false;
}

int RecordModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return rootNode->children.size();
    }

    const RecordNode* parentNode = static_cast<const RecordNode*>(parent.internalPointer());

    return parentNode->children.size();
}

int RecordModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant RecordModel::data(const QModelIndex &index, int role) const
{
    RecordNode* currentNode = static_cast<RecordNode*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
    {
        if(index.column() == 0) {
            QString prefix;
            switch (currentNode->level) {
            case RecordModel::FundLevel:
                prefix = tr("F. ");
                break;
            case RecordModel::InventoryLevel:
                prefix = tr("Inv. ");
                break;
            case RecordModel::RecordLevel:
                prefix = tr("R. ");
                break;
            }

            return QVariant(prefix + currentNode->number.toString());

        } else if(index.column() == 1) {
            return currentNode->comment;
        }  else if(index.column() == 2) {
            return currentNode->id;
        } else if(index.column() == 3) {
            return currentNode->name;
        }
        break;
    }
    case Qt::EditRole:
        if(index.column() == 0)
            return currentNode->number;
        break;
    case Qt::DecorationRole:
        if(index.column() == 0)
            switch (currentNode->level) {
            case RecordModel::FundLevel:
                return QIcon(":/icons/icons/folder-16.png");
                break;
            case RecordModel::InventoryLevel:
                return QIcon(":/icons/icons/folder-16.png");
                break;
            case RecordModel::RecordLevel:
                return QIcon(":/icons/icons/record-16.png");
                break;
            }
        break;
    case Qt::UserRole: // id
        return currentNode->id;
        break;
    }

    return QVariant();
}

bool RecordModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    RecordNode* currentNode = static_cast<RecordNode*>(index.internalPointer());

    switch (role) {
    case Qt::EditRole:
    {
        if(value != index.data()) {
            QSqlQuery query;

            QString field;
            if(index.column() == 0) {
                field = "number";
            } else if (index.column() == 1) {
                field = "comment";
            } else if (index.column() == 2) {
                return false; // id
            } else if (index.column() == 3) {
                if(currentNode->level != FundLevel)
                    return false;
                field = "name";
            }

            switch (currentNode->level) {
            case RecordModel::FundLevel:
                query.prepare(QString("UPDATE pad_fund SET %1=? WHERE id=?").arg(field));
                break;
            case RecordModel::InventoryLevel:
                query.prepare(QString("UPDATE pad_inventory SET %1=? WHERE id=?").arg(field));
                break;
            case RecordModel::RecordLevel:
                query.prepare(QString("UPDATE pad_record SET %1=? WHERE id=?").arg(field));
                break;
            }

            query.bindValue(0, value);
            query.bindValue(1, currentNode->id);
            query.exec();

            if(query.isActive()) {
                if(index.column() == 0) {
                    currentNode->number.setValue(value);
                } else if (index.column() == 1) {
                    currentNode->comment.setValue(value);
                } else if (index.column() == 3) {
                    currentNode->name.setValue(value);
                }

                emit dataChanged(index, index);

                return true;
            }

            qDebug() << query.lastError().text();
        }
    }
        break;
    }

    return false;
}

bool RecordModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
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
