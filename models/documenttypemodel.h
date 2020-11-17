#ifndef DOCUMENTTYPEMODEL_H
#define DOCUMENTTYPEMODEL_H

#include "models/standardreferencemodel.h"

class DocumentTypeModel : public StandardReferenceModel
{

    Q_OBJECT
public:
    DocumentTypeModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // DOCUMENTTYPEMODEL_H
