#include "protocolmodel.h"

#include <QDebug>
#include <QSqlRecord>

ProtocolModel::ProtocolModel(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable("pad_protocol");
    setEditStrategy(QSqlTableModel::OnRowChange);

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Record"));
    setHeaderData(2, Qt::Horizontal, tr("Number"));
    setHeaderData(3, Qt::Horizontal, tr("Pages"));
    setHeaderData(4, Qt::Horizontal, tr("Title"));
    setHeaderData(5, Qt::Horizontal, tr("Date"));
    setHeaderData(6, Qt::Horizontal, tr("Comment"));
}

void ProtocolModel::setRecordId(const QVariant &id)
{
    m_recordId = id;
    setFilter("record_id=" +id.toString());
};
