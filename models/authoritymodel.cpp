#include "authoritymodel.h"

#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

AuthorityModel::AuthorityModel(QObject *parent) : CollectionModel(parent)
{
    m_rootNode = new AT_Node;
    m_nodeList = new AT_NodeList;

    setHeaderData(0, Qt::Horizontal, tr("Public Authorities"));
}

AuthorityModel::~AuthorityModel()
{
    clear();
    delete m_rootNode;
    delete m_nodeList;
}

void AuthorityModel::clear()
{
    beginResetModel();
    for(int i = 0; i< m_nodeList->size(); ++i) {
        delete m_nodeList->at(i);
    }

    m_nodeList->clear();
    m_nodeList->squeeze();

    endResetModel();
}

int AuthorityModel::itemMaxNum(int column, const QRegExp &rule) const
{
    int max = 1;
    for(int i = 0; i < rowCount(rootItem()); ++i) {
        int num = 0;
        QModelIndex v = index(i, column, rootItem());

        if (rule.isEmpty() || v.data().toString().contains(rule)) {
            num = v.data().toString().remove(QRegExp("\\D+")).toInt();
        }

        if (num >= max) {
            max = num + 1;
        }
    }

    return max;
}

void AuthorityModel::select()
{
    beginResetModel();

    clear();
    setupModelData();

    endResetModel();
}

void AuthorityModel::setupModelData()
{
    QSqlQuery query("SELECT id, name FROM pad_authority ORDER BY name ASC");

    while (query.next()) {
        AT_Node *node = new AT_Node;

        node->append(query.value(query.record().indexOf("id")));
        node->append(query.value(query.record().indexOf("name")));

        m_nodeList->append(node);
    }
}

Qt::ItemFlags AuthorityModel::flags(const QModelIndex &index) const
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

QModelIndex AuthorityModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_rootNode);
    }

    return createIndex(row, column, m_nodeList->at(row));
}

bool AuthorityModel::insertRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;

    QVariant pa_name = tr("Public authority %1").arg(itemMaxNum(0, QRegExp("\\D+\\s\\D+\\d+")));

    query.prepare("INSERT INTO pad_authority(name) VALUES (?)");
    query.bindValue(0, pa_name.toString());
    query.exec();

    if(query.isActive()) {

       beginInsertRows(parent, row, row + count -1);

       AT_Node *node = new AT_Node();
       node->append(QVariant(query.lastInsertId()));
       node->append(pa_name);

       m_nodeList->append(node);

       endInsertRows();

       return true;
    }

    qDebug() << query.lastError().text();

    return false;
}

QVariant AuthorityModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex AuthorityModel::parent(const QModelIndex &index) const
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

bool AuthorityModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;

    for(int i = row; i < (row + count); ++i) {
        query.prepare("DELETE FROM pad_authority WHERE id=(?)");
        query.bindValue(0, m_nodeList->at(row)->at(0));
        query.exec();

        if(query.isActive()) {
           beginRemoveRows(parent, row, row + count -1);

           for(int j = 0; j< m_nodeList->at(i)->size(); ++j) {
               m_nodeList->at(i)->removeAt(j);
           }

           m_nodeList->removeAt(i);

           endRemoveRows();
        } else {
            qDebug() << query.lastError().text();

            return false;
        }
    }

    return true;
}

QModelIndex AuthorityModel::rootItem() const
{
    return index(0, 0, QModelIndex());
}

int AuthorityModel::rowCount(const QModelIndex &parent) const
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

int AuthorityModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant AuthorityModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            if(index.parent().isValid()) {
                const AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());
                return currentNode->at(1);
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
                if(index == rootItem())
                    return QIcon(":/icons/icons/folder-16.png");

                return QIcon(":/icons/icons/icon-16.png");
            }
        break;
        // return id of item
        case Qt::UserRole:
            if (index.column() == 0) {
                const AT_Node* currentNode = static_cast<AT_Node*>(index.internalPointer());
                return currentNode->at(0);
            }
        break;
    }

    return QVariant();
}

bool AuthorityModel::setData(const QModelIndex &index, const QVariant &value, int role){
    switch (role) {
        case Qt::EditRole:
            if(value.toString().size() > 0) {
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
            }
        break;
    }

    return false;
}

bool AuthorityModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
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
