#ifndef DOCUMENTTYPEMODEL_H
#define DOCUMENTTYPEMODEL_H

#include "models/standardreferencemodel.h"

#include <QSqlTableModel>

class DocumentTypeModel : public StandardReferenceModel
{

    Q_OBJECT
public:
    DocumentTypeModel(QWidget *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    QSqlTableModel *m_internalModel;

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // DOCUMENTTYPEMODEL_H
