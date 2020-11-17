#ifndef SUBJECTMODEL_H
#define SUBJECTMODEL_H

#include "models/standardreferencemodel.h"

class SubjectModel : public StandardReferenceModel
{
    Q_OBJECT
public:
    SubjectModel(QObject *parent = nullptr);

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // SUBJECTMODEL_H
