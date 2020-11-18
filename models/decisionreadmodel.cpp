#include "decisionreadmodel.h"

#include <QColor>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

DecisionReadModel::DecisionReadModel(QObject *parent)
    : ReferenceModel(parent)
{
    m_internalModel = new QSqlQueryModel;
}

DecisionReadModel::~DecisionReadModel()
{
    clear();
    delete m_internalModel;
}

void DecisionReadModel::clear()
{
    beginResetModel();
    m_internalModel->clear();
    endResetModel();
}

void DecisionReadModel::select()
{
    beginResetModel();

    if(authorityId()) {
        where("pad_decision.authority_id=" + QString::number(authorityId()));
    }

    QSqlQuery query;
    query.prepare(QString(" \
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
    ).arg(filter().isEmpty() ? QString() : filter()));

    if (query.exec()) {
        m_internalModel->setQuery(query);
    } else {
        qDebug () << query.lastError().text();
    }

    endResetModel();
}

int DecisionReadModel::total()
{
    if(authorityId()) {
        where("authority_id=" + QString::number(authorityId()));
    }

    QSqlQuery query;
    query.prepare(
                QString("SELECT COUNT(id) FROM pad_decision %1")
                .arg(filter().isEmpty() ? QString() : filter()));

    if (query.exec()) {
        query.first();
        return query.value(0).toInt();
    }

    return 0;
}

QVariant DecisionReadModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_internalModel->headerData(section, orientation, role);
}

QModelIndex DecisionReadModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, nullptr);
    }

    return QModelIndex();
}

QModelIndex DecisionReadModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int DecisionReadModel::rowCount(const QModelIndex &parent) const
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->rowCount(internalParent);
}

int DecisionReadModel::columnCount(const QModelIndex &parent) const
{
    QModelIndex internalIndex = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->columnCount(internalIndex);
}

QVariant DecisionReadModel::data(const QModelIndex &index, int role) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());

    if(role == Qt::BackgroundColorRole) {
            return QColor(internalIndex.siblingAtColumn(5).data().toString());
    }

    return m_internalModel->data(internalIndex, role);
}
