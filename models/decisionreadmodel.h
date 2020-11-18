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
};

#endif // DECISIONREADMODEL_H
