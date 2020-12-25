#ifndef SUBJTYPEMODEL_H
#define SUBJTYPEMODEL_H

#include "models/standardreferencemodel.h"

class SubjtypeModel : public StandardReferenceModel
{
    Q_OBJECT
public:
    SubjtypeModel(QObject *parent = nullptr);

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // SUBJTYPEMODEL_H
