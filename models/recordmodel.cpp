#include "recordmodel.h"

#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

RecordModel::RecordModel(QObject *parent) : ReferenceModel(parent)
{
    rootNode = new RecordNode;
    setHeaderData(0, Qt::Horizontal, tr("Archive records"));
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

    switch (level) {
    case RecordModel::FundLevel:
    {
        QString filter = m_authorityId > 0 ? QString(" WHERE authority_id=") + QString::number(m_authorityId) : QString();
        query.prepare(QString("SELECT id, number, comment, name FROM pad_fund %1 ORDER BY CAST(number AS UNSIGNED) ASC").arg(filter));
    }
        break;
    case RecordModel::InventoryLevel:
        query.prepare("SELECT id, number, comment FROM pad_inventory WHERE fund_id=? ORDER BY CAST(number AS UNSIGNED) ASC");
        query.bindValue(0, parentNode->id.toInt());
        break;
    case RecordModel::RecordLevel:
        query.prepare("SELECT id, number, comment FROM pad_record WHERE inventory_id=? ORDER BY CAST(number AS UNSIGNED) ASC");
        query.bindValue(0, parentNode->id.toInt());
        break;
    }

    query.exec();

    if(query.isActive()) {
        while(query.next()) {
            RecordNode *node = new RecordNode();

            node->id = query.record().value(0);
            node->number.setValue(query.record().value(1));
            node->comment.setValue(query.record().value(2).toString());

            node->level = level;
            node->row = query.at();
            node->mapped = false;
            node->parent = (level != FundLevel) ? parentNode : nullptr;

            parentNode->mapped = (level != FundLevel);
            parentNode->children.append(node);

            // store fund name
            if(level == FundLevel)
                fundNames.insert(node, query.record().value(3));
        }
    } else {
        qDebug() << query.lastError().text();
    }
}

Qt::ItemFlags RecordModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
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
        if(m_authorityId == 0) {
            return false;
        }
        name = tr("New fund");
        query.prepare("INSERT INTO pad_fund(authority_id, number) VALUES (?,?)");
        query.bindValue(0, m_authorityId);
        query.bindValue(1, name);
    } else if(parentNode->level == RecordModel::FundLevel) {
        name = tr("New inventory");
        query.prepare("INSERT INTO pad_inventory(number, fund_id) VALUES (?,?)");
        query.bindValue(0, name);
        query.bindValue(1, parentNode->id);
    } else if(parentNode->level == RecordModel::InventoryLevel) {
        name = tr("New record");
        query.prepare("INSERT INTO pad_record(number, inventory_id) VALUES (?,?)");
        query.bindValue(0, name);
        query.bindValue(1, parentNode->id);
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
        } else {
            qDebug() << query.lastError().text();

            return false;
        }
    }

    return true;
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
    return 1;
}

QVariant RecordModel::data(const QModelIndex &index, int role) const
{
    RecordNode* currentNode = static_cast<RecordNode*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:
        {
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
            break;
        }
        case Qt::EditRole:
            return currentNode->number.toString();
            break;
        case Qt::DecorationRole:
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
        case ReferenceModel::CommentRole: // comment
            return currentNode->comment;
            break;
        case ReferenceModel::InfoRole: // fund name
            return fundNames.value(currentNode);
            break;
    }

    return QVariant();
}

bool RecordModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(value.toString().size() == 0) {
        return false;
    }

    RecordNode* currentNode = static_cast<RecordNode*>(index.internalPointer());

    switch (role) {
    case Qt::EditRole:
    {
        if(value != currentNode->number) {
            QSqlQuery query;

            switch (currentNode->level) {
            case RecordModel::FundLevel:
                query.prepare("UPDATE pad_fund SET number=? WHERE id=?");
                break;
            case RecordModel::InventoryLevel:
                query.prepare("UPDATE pad_inventory SET number=? WHERE id=?");
                break;
            case RecordModel::RecordLevel:
                query.prepare("UPDATE pad_record SET number=? WHERE id=?");
                break;
            }

            query.bindValue(0, value);
            query.bindValue(1, currentNode->id);
            query.exec();

            if(query.isActive()) {
                currentNode->number.setValue(value);
                emit dataChanged(index, index);

                return true;
            }

            qDebug() << query.lastError().text();
        }
    }
        break;
    case ReferenceModel::CommentRole: // update comment
    {
        QSqlQuery query;

        switch (currentNode->level) {
        case RecordModel::FundLevel:
            query.prepare("UPDATE pad_fund SET comment=? WHERE id=?");
            break;
        case RecordModel::InventoryLevel:
            query.prepare("UPDATE pad_inventory SET comment=? WHERE id=?");
            break;
        case RecordModel::RecordLevel:
            query.prepare("UPDATE pad_record SET comment=? WHERE id=?");
            break;
        }

        query.bindValue(0, value);
        query.bindValue(1, currentNode->id);
        query.exec();

        if(query.isActive()) {
            currentNode->comment.setValue(value);
            emit dataChanged(index, index);

            return true;
        }

        qDebug() << query.lastError().text();
    }
        break;
    case ReferenceModel::InfoRole: // update fund name
    {
        if(currentNode->level == RecordModel::FundLevel) {
            QSqlQuery query;
            query.prepare("UPDATE pad_fund SET name=? WHERE id=?");
            query.bindValue(0, value);
            query.bindValue(1, currentNode->id);
            query.exec();

            if(query.isActive()) {
                fundNames.insert(currentNode, value);

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
