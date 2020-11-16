#include "refbasemodel.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

RefBaseModel::RefBaseModel(QObject *parent) : QAbstractItemModel(parent)
{
    m_root = new QStandardItem(tr("Root"));
}

RefBaseModel::~RefBaseModel()
{
    delete m_root;
}

void RefBaseModel::setQuery(const QString &query)
{
    QSqlQuery q;
    q.prepare(query);

    q.exec();

    if(q.isActive()) {
        int i = 0;
        while (q.next()) {
            QStandardItem *v = new QStandardItem;
            QList<QStandardItem*> column;
            for(int i = 0; i < q.record().count(); ++i) {
                QStandardItem *item = new QStandardItem(q.value(i).toString());
                column.append(item);
            }
            //v->setColumnCount(column.size());
            v->appendColumn(column);
            v->appendRows(column);


            qDebug() << v->child(0)->text();
            m_root->setChild(i, v);
            ++i;
        }
        //m_root->setChild(0, v);

        qDebug() << m_root->child(0)->text();
        qDebug() << m_root->child(1)->text();
        qDebug() << m_root->rowCount();
    } else {
        qDebug() << q.lastError().text();
    }
}

void RefBaseModel::setRootText(const QString &text)
{
    m_root->setText(text);
}

QVariant RefBaseModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex RefBaseModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, m_root);
    }

    return createIndex(row, column, m_root->child(0));
}

QModelIndex RefBaseModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    QStandardItem *node = static_cast<QStandardItem*>(index.internalPointer());
    if(node) {
        return createIndex(0, 0, node);
    }

    return QModelIndex();
}

int RefBaseModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        //qDebug() << "rows:" <<1;
        return 1;
    }

    QStandardItem *node = static_cast<QStandardItem*>(parent.internalPointer());
    if(node) {
        return node->hasChildren() ? node->child(0)->rowCount() : 0;
    }

    return 0;
}

int RefBaseModel::columnCount(const QModelIndex &) const
{
    //return m_root->columnCount();
    return 1;
}

QVariant RefBaseModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    {
        QStandardItem *node = static_cast<QStandardItem*>(index.internalPointer());
        return QVariant(node->text());
    }
        break;
    }

    return QVariant();
}


bool RefBaseModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
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

