#include "doctypemodel.h"

#include "widgets/colorrect.h"

DoctypeModel::DoctypeModel()
{
    setTable("pad_doctype");
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setSort(0, Qt::AscendingOrder);

    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Color"));

    connect(this, &QSqlTableModel::primeInsert, this, &DoctypeModel::setDefaults);
}

void DoctypeModel::setDefaults(int, QSqlRecord &record)
{
    record.setValue("name", tr("New document type"));
    record.setGenerated("name", true);
}


QVariant DoctypeModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DecorationRole && index.column() == 2) {
        return ColorRect::pixmap(QColor(index.data().toString()), 16);
    }

    return QSqlTableModel::data(index, role);
}
