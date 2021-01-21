#ifndef PROTOCOLMODEL_H
#define PROTOCOLMODEL_H

#include <QSqlTableModel>

class ProtocolModel: public QSqlTableModel
{
    Q_OBJECT
public:
    ProtocolModel(QObject *parent = nullptr);
    void setRecordId(const QVariant &id);

    QVariant recordId() const { return m_recordId; };

private:
    QVariant m_recordId;
};

#endif // PROTOCOLMODEL_H
