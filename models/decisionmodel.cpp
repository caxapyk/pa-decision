#include "decisionmodel.h"

#include <QColor>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

DecisionModel::DecisionModel()
{
    m_nodeList = new NodeList;

    setHeaderData(DecisionModel::Date, Qt::Horizontal, tr("Date"));
    setHeaderData(DecisionModel::Number, Qt::Horizontal, tr("Number"));
    setHeaderData(DecisionModel::Title, Qt::Horizontal, tr("Title"));
    setHeaderData(DecisionModel::ARecord, Qt::Horizontal, tr("Archive record"));
    setHeaderData(DecisionModel::Comment, Qt::Horizontal, tr("Comment"));
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

void DecisionModel::andWhere(const QString &condition)
{

}

void DecisionModel::orWhere(const QString &condition)
{

}

void DecisionModel::where(const QString &condition)
{

}

QString DecisionModel::field(const QModelIndex &index) const
{
    QString field;

    if(index.column() == Column::Date)
        field = "date";
    else if(index.column() == Column::Number)
        field = "number";
    else if(index.column() == Column::Title)
        field = "title";
    else if(index.column() == Column::Comment)
         field = "comment";

    return field;
}

void DecisionModel::setupModelData()
{
    QSqlQuery query;
    query.prepare(
            QString(" \
                    SELECT \
                    pad_decision.id, \
                    pad_decision.date, \
                    pad_decision.number, \
                    pad_decision.title, \
                    pad_decision.comment, \
                    pad_doctype.color AS color \
                    FROM pad_decision \
                    LEFT JOIN pad_doctype ON pad_decision.doctype_id=pad_doctype.id \
                    %1 ORDER BY pad_decision.id DESC"
            ).arg(cond.isEmpty() ? QString() : "WHERE " + cond));

    query.exec();

    while (query.next()) {
        Node *node = new Node;

        for(int i = 0; i < query.record().count(); ++i) {
            node->insert(query.record().fieldName(i), query.record().value(i));
        }

        m_nodeList->append(node);
    }
}

int DecisionModel::columnCount(const QModelIndex &) const
{
    return ColumnCount;
}

QVariant DecisionModel::data(const QModelIndex &index, int role) const
{
    const Node* currentNode = static_cast<Node*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
        return currentNode->value(field(index));
    break;
    case Qt::EditRole:
        return data(index, Qt::DisplayRole);
    break;
    case Qt::BackgroundColorRole:
        return QColor(currentNode->value("color").toString());
    }

    return QVariant();
}

Qt::ItemFlags DecisionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()  || index.column() == DecisionModel::ARecord) {
        return QAbstractItemModel::flags(index);
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QModelIndex DecisionModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column, m_nodeList->at(row));
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
    } else if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QVariant();
}

QModelIndex DecisionModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int DecisionModel::rowCount(const QModelIndex &) const
{
    return m_nodeList->size();
}

bool DecisionModel::setData(const QModelIndex &index, const QVariant &value, int role){
    switch (role) {
        case Qt::EditRole:
            if(value.toString().size() > 0) {
                Node* currentNode = static_cast<Node*>(index.internalPointer());

                QSqlQuery query;
                QString f = field(index);

                query.prepare(QString("UPDATE pad_decision SET %1=? WHERE id=?").arg(f));
                query.bindValue(0, value);
                query.bindValue(1, currentNode->value("id"));
                query.exec();

                if(query.isActive()) {
                    currentNode->insert(f, value);
                    emit dataChanged(index, index);

                    return true;
                }

                qDebug() << query.lastError().text();
            }
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
