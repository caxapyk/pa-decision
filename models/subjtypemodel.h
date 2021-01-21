#ifndef SUBJTYPEMODEL_H
#define SUBJTYPEMODEL_H

#include <QSqlTableModel>

class SubjtypeModel : public QSqlTableModel
{
    Q_OBJECT
public:
    SubjtypeModel(QObject *parent = nullptr);

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // SUBJTYPEMODEL_H
