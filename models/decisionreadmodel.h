#ifndef DECISIONREADMODEL_H
#define DECISIONREADMODEL_H

#include "models/referencemodel.h"

#include <QSqlQueryModel>

class DecisionReadModel : public ReferenceModel
{
    Q_OBJECT

public:
    explicit DecisionReadModel(QObject *parent = nullptr);
    ~DecisionReadModel();

    int total();
    bool primeInsert(
            const QVariant &record_id,
            const QVariant &authority_id,
            const QVariant &doctype_id,
            const QVariant &protocol_id,
            const QVariant &date,
            const QVariant &number,
            const QVariant &title,
            const QVariant &access,
            const QVariant &content,
            const QVariant &comment);

    bool primeDelete(int id);

    QVariant lastInsertId() { return m_lastInsertId; };

    void clear() override;
    void select() override;

    QSqlQueryModel *sourceModel() { return m_internalModel; };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QSqlQueryModel *m_internalModel;
    QVariant m_lastInsertId;
};

#endif // DECISIONREADMODEL_H
