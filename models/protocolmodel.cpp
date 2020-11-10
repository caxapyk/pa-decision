#include "protocolmodel.h"

#include <QDate>
#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

ProtocolModel::ProtocolModel(QObject *parent) : ReferenceModel(parent)
{
    m_nodeList = new NodeList;

    setHeaderData(0, Qt::Horizontal, tr("Number"));
    setHeaderData(1, Qt::Horizontal, tr("Date"));
    setHeaderData(2, Qt::Horizontal, tr("Title"));
    setHeaderData(3, Qt::Horizontal, tr("Comment"));
    setHeaderData(4, Qt::Horizontal, tr("ID"));
}

ProtocolModel::~ProtocolModel()
{
    clear();
    delete m_nodeList;
}

void ProtocolModel::clear()
{
    beginResetModel();
    for(int i = 0; i< m_nodeList->size(); ++i) {
        delete m_nodeList->at(i);
    }

    m_nodeList->clear();
    m_nodeList->squeeze();

    clearFilter();

    endResetModel();
}

void ProtocolModel::select()
{
    beginResetModel();

    clear();
    setupModelData();

    endResetModel();
}

void ProtocolModel::setupModelData()
{
    QSqlQuery query;

    if(authorityId() && filter().isEmpty())
        where("authority_id=" + QString::number(authorityId()));
    else if(authorityId() && !filter().isEmpty())
        andWhere("authority_id=" + QString::number(authorityId()));

    query.prepare(QString("SELECT number, date, title, comment, id FROM pad_protocol %1 ORDER BY number ASC").arg(filter()));
    query.exec();

    while (query.next()) {
        Node *node = new Node;

        node->append(query.value(query.record().indexOf("number")));
        node->append(query.value(query.record().indexOf("date")));
        node->append(query.value(query.record().indexOf("title")));
        node->append(query.value(query.record().indexOf("comment")));
        node->append(query.value(query.record().indexOf("id")));

        m_nodeList->append(node);
    }
}

Qt::ItemFlags ProtocolModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QAbstractItemModel::flags(index);
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QModelIndex ProtocolModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_nodeList->at(row));
    }

    return QModelIndex();
}

bool ProtocolModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(mSetRow.isEmpty()) {
        return false;
    }

    QSqlQuery query;

    QList<QString> v;
    for(int i = 0; i < mSetRow.size(); ++i) {
        v.append("?");
    }

    query.prepare(QString("INSERT INTO pad_protocol(%1) VALUES (%2)")
                  .arg(mSetRow.keys().join(","))
                  .arg(v.join(",")));

    for(int i = 0; i < mSetRow.size(); ++i) {
        QString v = mSetRow.values().at(i).toString();
        if(mSetRow.keys().at(i) == "date") {
            v = QDate().fromString(v, "dd.MM.yyyy").toString("yyyy-MM-dd");
        }

        query.bindValue(i, v);
    }

    query.exec();

    if(query.isActive()) {
        beginInsertRows(parent, row, row + count -1);

        Node *node = new Node();
        node->append(mSetRow.contains("number") ? mSetRow.value("number") : QVariant());
        node->append(mSetRow.contains("date") ? mSetRow.value("date") : QVariant());
        node->append(mSetRow.contains("title") ? mSetRow.value("title") : QVariant());
        node->append(mSetRow.contains("comment") ? mSetRow.value("comment") : QVariant());
        node->append(QVariant(query.lastInsertId()));

        m_nodeList->append(node);

        endInsertRows();

        return true;
    }

    qDebug() << query.lastError().text();

    return false;
}

QVariant ProtocolModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex ProtocolModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool ProtocolModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;

    for(int i = row; i < (row + count); ++i) {
        query.prepare("DELETE FROM pad_protocol WHERE id=?");
        query.bindValue(0, m_nodeList->at(row)->at(4));
        query.exec();

        if(query.isActive()) {
            beginRemoveRows(parent, row, row + count -1);

            for(int j = 0; j< m_nodeList->at(i)->size(); ++j) {
                m_nodeList->at(i)->removeAt(j);
            }

            m_nodeList->removeAt(i);

            endRemoveRows();

            return true;
        }
         qDebug() << query.lastError().text();
    }

    return false;
}

int ProtocolModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_nodeList->size();
    }

    return 0;
}

int ProtocolModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant ProtocolModel::data(const QModelIndex &index, int role) const
{
    const Node* currentNode = static_cast<Node*>(index.internalPointer());
    switch (role) {
        case Qt::DisplayRole:
            return currentNode->at(index.column());
        break;
        case Qt::EditRole:
            return data(index, Qt::DisplayRole);
        break;
        case Qt::UserRole: // id
            return currentNode->at(4);
        break;
    }

    return QVariant();
}

bool ProtocolModel::setData(const QModelIndex &index, const QVariant &value, int role){
    Node* currentNode = static_cast<Node*>(index.internalPointer());

    switch (role) {
    case Qt::EditRole:
        QSqlQuery query;

        QString field;
        if(index.column() == 0) {
            field = "number";
        } else if (index.column() == 1) {
            field = "date";
        } else if (index.column() == 2) {
            field = "title";
        } else if (index.column() == 3) {
            field = "comment";
        } else if (index.column() == 4) {
            return false; // id
        }

        query.prepare(QString("UPDATE pad_protocol SET %1=? WHERE id=?").arg(field));
        query.bindValue(0, value);
        query.bindValue(1, currentNode->at(4));
        query.exec();

        if(query.isActive()) {
            currentNode->replace(index.column(), value);
            emit dataChanged(index, index);

            return true;
        }

        qDebug() << query.lastError().text();
        break;
    }

    return false;
}

bool ProtocolModel::setRow(int row, QMap<QString, QVariant> &values, const QModelIndex &parent)
{
    mSetRow.swap(values);

    return insertRow(row, parent);
}

bool ProtocolModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
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
