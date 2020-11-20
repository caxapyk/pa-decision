#include "decisionreadmodel.h"

#include <QColor>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
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
    query.prepare(tr(" \
                          SELECT \
                          pad_decision.id, \
                          pad_doctype.name AS doctype, \
                          pad_decision.date, \
                          pad_decision.number, \
                          pad_decision.title, \
                          ( \
                               SELECT \
                               CONCAT(\"F.\", pad_fund.number, \" Inv.\", pad_inventory.number, \" R.\", pad_record.number) \
                               FROM pad_record \
                               JOIN pad_inventory ON pad_record.inventory_id=pad_inventory.id \
                               JOIN pad_fund ON pad_inventory.fund_id=pad_fund.id \
                               WHERE pad_record.id=pad_decision.record_id \
                               ORDER BY CAST(pad_fund.number AS UNSIGNED), CAST(pad_inventory.number AS UNSIGNED), CAST(pad_record.number AS UNSIGNED) \
                          ) AS record, \
                          CONCAT(\"№\", pad_protocol.number, \" from \", pad_protocol.date, \" \", pad_protocol.title) AS protocol, \
                          pad_decision.comment, \
                          pad_decision.content, \
                          pad_decision.access, \
                          pad_doctype.color AS color \
                          FROM pad_decision \
                          LEFT JOIN pad_doctype ON pad_decision.doctype_id=pad_doctype.id \
                          LEFT JOIN pad_authority ON pad_decision.authority_id=pad_authority.id \
                          LEFT JOIN pad_protocol ON pad_decision.protocol_id=pad_protocol.id \
                          %1 ORDER BY pad_decision.id DESC"
    ).arg(filter().isEmpty() ? QString() : filter()));

    if (query.exec()) {
        m_internalModel->setQuery(query);
    } else {
        qDebug () << query.lastError().text();
    }

    endResetModel();

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Document type"));
    setHeaderData(2, Qt::Horizontal, tr("Date"));
    setHeaderData(3, Qt::Horizontal, tr("Number"));
    setHeaderData(4, Qt::Horizontal, tr("Title"));
    setHeaderData(5, Qt::Horizontal, tr("Archive record"));
    setHeaderData(6, Qt::Horizontal, tr("Protocol"));
    setHeaderData(7, Qt::Horizontal, tr("Comment"));
    setHeaderData(8, Qt::Horizontal, tr("Content"));
    setHeaderData(9, Qt::Horizontal, tr("Access"));
    setHeaderData(10, Qt::Horizontal, tr("Color"));
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

bool DecisionReadModel::save(
        const QVariant &record_id,
        const QVariant &authority_id,
        const QVariant &doctype_id,
        const QVariant &protocol_id,
        const QVariant &date,
        const QVariant &number,
        const QVariant &title,
        const QVariant &access,
        const QVariant &content,
        const QVariant &comment,
        const QVariant &id)
{
    QSqlQuery query;

    if(id.isValid()) {
        query.prepare(("UPDATE pad_decision SET record_id=?, authority_id=?, doctype_id=?, protocol_id=?, date=?, number=?, title=?, access=?, content=?, comment=? WHERE id=?"));
    } else {
        query.prepare(("INSERT INTO pad_decision (record_id, authority_id, doctype_id, protocol_id, date, number, title, access, content, comment) VALUES(?,?,?,?,?,?,?,?,?,?)"));
    }
    query.bindValue(0, record_id);
    query.bindValue(1, authority_id);
    query.bindValue(2, doctype_id);
    query.bindValue(3, protocol_id);
    query.bindValue(4, date);
    query.bindValue(5, number);
    query.bindValue(6, title);
    query.bindValue(7, access);
    query.bindValue(8, content);
    query.bindValue(9, comment);

    if(id.isValid()) {
        query.bindValue(10, id);
    }

    if(query.exec()) {
        if(!id.isValid()) {
            m_lastInsertId = query.lastInsertId();
        }
        return true;
    }

    qDebug () << query.lastError().text();

    return false;
}

bool DecisionReadModel::primeDelete(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM pad_decision WHERE id=?");
    query.bindValue(0, id);

    if(query.exec()) {
        return true;
    }

    qDebug () << query.lastError().text();

    return false;
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
        QVariant color = internalIndex.siblingAtColumn(m_internalModel->record().indexOf("color")).data();
        return QColor(color.toString());
    }

    return m_internalModel->data(internalIndex, role);
}

bool DecisionReadModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return m_internalModel->setHeaderData(section, orientation, value, role);
}
