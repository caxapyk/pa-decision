#include "documenttypemodel.h"

#include "widgets/colorrect.h"

#include <QDebug>
#include <QColor>
#include <QSqlRecord>

DocumentTypeModel::DocumentTypeModel(QObject *parent) :
    StandardReferenceModel(parent)
{
   setTable("pad_doctype");

   setHeaderData(0, Qt::Horizontal, tr("ID"));
   setHeaderData(1, Qt::Horizontal, tr("Name"));
   //setHeaderData(2, Qt::Horizontal, tr("Color"));

   connect(sourceModel(), &QSqlTableModel::primeInsert, this, &DocumentTypeModel::setDefaults);
}

QVariant DocumentTypeModel::data(const QModelIndex &index, int role) const
{
    /*if(role == Qt::DecorationRole && index.column() == 1){
        return ColorRect::pixmap(QColor(index.siblingAtColumn(2).data().toString()), 16);
    }*/

    return StandardReferenceModel::data(index, role);
}

void DocumentTypeModel::setDefaults(int, QSqlRecord &record)
{
    record.setValue("name", tr("Document type %1").arg(itemMaxNum(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}
