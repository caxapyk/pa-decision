#include "documenttypemodel.h"

#include "widgets/colorrect.h"

#include <QColor>
#include <QDebug>
#include <QIcon>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

DocumentTypeModel::DocumentTypeModel()
{
    m_nodeList = new NodeList;

    setHeaderData(0, Qt::Horizontal, tr("Type"));
    setHeaderData(1, Qt::Horizontal, tr("Color"));
    setHeaderData(2, Qt::Horizontal, tr("ID"));
}

DocumentTypeModel::~DocumentTypeModel()
{
    clear();
    delete m_nodeList;
}

void DocumentTypeModel::clear()
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

int DocumentTypeModel::itemMaxNum(int column, const QRegExp &rule) const
{
    int max = 1;
    for(int i = 0; i < rowCount(); ++i) {
        int num = 0;
        QModelIndex v = index(i, column);

        if (rule.isEmpty() || v.data().toString().contains(rule)) {
            num = v.data().toString().remove(QRegExp("\\D+")).toInt();
        }

        if (num >= max) {
            max = num + 1;
        }
    }

    return max;
}

void DocumentTypeModel::select()
{
    beginResetModel();

    clear();
    setupModelData();

    endResetModel();
}

void DocumentTypeModel::setupModelData()
{
    QSqlQuery query;

    if(authorityId() == 0) {
        query.prepare("SELECT name, color, id FROM pad_doctype ORDER BY name ASC");
    } else {
        query.prepare("SELECT pad_doctype.name, pad_doctype.color, pad_doctype.id FROM pad_doctype JOIN pad_decision ON pad_decision.doctype_id=pad_doctype.id WHERE pad_decision.authority_id=? ORDER BY pad_doctype.name ASC");
        query.bindValue(0, authorityId());
    }

    query.exec();

    while (query.next()) {
        Node *node = new Node;

        node->append(query.value(query.record().indexOf("name")));
        node->append(query.value(query.record().indexOf("color")));
        node->append(query.value(query.record().indexOf("id")));

        m_nodeList->append(node);
    }
}

Qt::ItemFlags DocumentTypeModel::flags(const QModelIndex &index) const
{
    if(index.column() == 0 || index.column() == 1) {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return ReferenceModel::flags(index);
}

QModelIndex DocumentTypeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_nodeList->at(row));
    }

    return QModelIndex();
}

bool DocumentTypeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;

    QVariant name = tr("Document type %1").arg(itemMaxNum(0, QRegExp("\\D+\\s\\D+\\d+")));

    query.prepare("INSERT INTO pad_doctype(name) VALUES (?)");
    query.bindValue(0, name.toString());
    query.exec();

    if(query.isActive()) {

        beginInsertRows(parent, row, row + count -1);

        Node *node = new Node();
        node->append(name);
        node->append(QVariant("white"));
        node->append(QVariant(query.lastInsertId()));

        m_nodeList->append(node);

        endInsertRows();

        return true;
    }

    qDebug() << query.lastError().text();

    return false;
}

QVariant DocumentTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex DocumentTypeModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool DocumentTypeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery query;

    for(int i = row; i < (row + count); ++i) {
        query.prepare("DELETE FROM pad_doctype WHERE id=?");
        query.bindValue(0, m_nodeList->at(row)->at(2));
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

int DocumentTypeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_nodeList->size();
    }

    return 0;
}

int DocumentTypeModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant DocumentTypeModel::data(const QModelIndex &index, int role) const
{
    const Node* currentNode = static_cast<Node*>(index.internalPointer());
    switch (role) {
        case Qt::DisplayRole:
            return currentNode->at(index.column());
        break;
        case Qt::EditRole:
            return data(index, Qt::DisplayRole);
        break;
        case Qt::DecorationRole:
            if(index.column() == 0) {
                return ColorRect::pixmap(QColor(currentNode->at(1).toString()), 16);
            }
        break;
        case Qt::UserRole: // id
            return currentNode->at(2);
        break;
    }

    return QVariant();
}

bool DocumentTypeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    Node* currentNode = static_cast<Node*>(index.internalPointer());

    switch (role) {
    case Qt::EditRole:
        if(value.toString().size() > 0) {
            QSqlQuery query;

            QString field;
            if(index.column() == 0) {
                field = "name";
            } else if (index.column() == 1) {
                field = "color";
            } else if (index.column() == 2) {
                return false; // id
            }

            query.prepare(QString("UPDATE pad_doctype SET %1=? WHERE id=?").arg(field));
            query.bindValue(0, value);
            query.bindValue(1, currentNode->at(2));
            query.exec();

            if(query.isActive()) {
                currentNode->replace(index.column(), value);
                emit dataChanged(index, index);

                return true;
            }

            qDebug() << query.lastError().text();
        }
        break;
    }

    return false;
}

bool DocumentTypeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
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
