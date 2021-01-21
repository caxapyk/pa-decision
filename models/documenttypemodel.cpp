#include "documenttypemodel.h"

#include "utils/itemcounter.h"
//#include "widgets/colorrect.h"

#include <QDebug>
#include <QColor>
#include <QSqlRecord>

DocumentTypeModel::DocumentTypeModel(QObject *parent) :
    QSqlTableModel(parent)
{
   setTable("pad_doctype");

   setHeaderData(0, Qt::Horizontal, tr("ID"));
   setHeaderData(1, Qt::Horizontal, tr("Name"));
   //setHeaderData(2, Qt::Horizontal, tr("Color"));

   connect(this, &QSqlTableModel::primeInsert, this, &DocumentTypeModel::setDefaults);
}

QVariant DocumentTypeModel::data(const QModelIndex &index, int role) const
{
    /*if(role == Qt::DecorationRole && index.column() == 1){
        return ColorRect::pixmap(QColor(index.siblingAtColumn(2).data().toString()), 16);
    }*/

    return QSqlTableModel::data(index, role);
}

void DocumentTypeModel::setDefaults(int, QSqlRecord &record)
{
    ItemCounter counter(this);

    record.setValue("name", tr("Document type %1").arg(counter.next(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}
