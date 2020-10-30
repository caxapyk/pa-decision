#ifndef DOCTYPEMODEL_H
#define DOCTYPEMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>

class DoctypeModel : public QSqlTableModel
{
    Q_OBJECT
public:
    DoctypeModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void setDefaults(int row, QSqlRecord &record);
};

#endif // DOCTYPEMODEL_H
