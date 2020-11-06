#include "documenttypemodel.h"

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
    QSqlQuery query("SELECT id, name, color FROM pad_doctype ORDER BY name ASC");

    while (query.next()) {
        Node *node = new Node;

        node->append(query.value(query.record().indexOf("id")));
        node->append(query.value(query.record().indexOf("name")));
        node->append(query.value(query.record().indexOf("color")));

        m_nodeList->append(node);
    }
}

Qt::ItemFlags DocumentTypeModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
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

int DocumentTypeModel::rowCount(const QModelIndex &parent) const
{
    return m_nodeList->size();
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
            if(index.column() == 0)
                return currentNode->at(1);
            else if(index.column() == 1)
                return currentNode->at(2);
        break;
        case Qt::EditRole:
            return data(index, Qt::DisplayRole);
        break;
        //case Qt::DecorationRole:
        //    return QIcon(":/icons/icons/icon-16.png");
        //break;
        case ReferenceModel::IDRole:
            if(index.parent().isValid()) {
                return currentNode->at(0);
            }
        break;
        case ReferenceModel::CommentRole:
            if(index.parent().isValid()) {
                return currentNode->at(3);
            }
        break;
    }

    return QVariant();
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
