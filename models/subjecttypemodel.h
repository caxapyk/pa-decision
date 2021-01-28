#ifndef SUBJTYPEMODEL_H
#define SUBJTYPEMODEL_H

#include <QSqlTableModel>

class SubjectTypeModel : public QSqlTableModel
{
    Q_OBJECT
public:
    SubjectTypeModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // SUBJTYPEMODEL_H
