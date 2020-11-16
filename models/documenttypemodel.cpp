#include "documenttypemodel.h"

#include "widgets/colorrect.h"

#include <QDebug>
#include <QColor>
#include <QSqlRecord>

DocumentTypeModel::DocumentTypeModel()
{
   m_internalModel = new QSqlTableModel;
   m_internalModel->setTable("pad_doctype");
   m_internalModel->setEditStrategy(QSqlTableModel::OnFieldChange);
   m_internalModel->setSort(0, Qt::AscendingOrder);

   setHeaderData(1, Qt::Horizontal, tr("ID"));
   setHeaderData(1, Qt::Horizontal, tr("Name"));
   setHeaderData(2, Qt::Horizontal, tr("Color"));

   connect(m_internalModel, &QSqlTableModel::primeInsert, this, &DocumentTypeModel::setDefaults);
}

DocumentTypeModel::~DocumentTypeModel()
{
    clear();
    delete m_internalModel;
}

void DocumentTypeModel::clear()
{
    beginResetModel();
    m_internalModel->clear();
    endResetModel();
}

void DocumentTypeModel::select()
{
    beginResetModel();
    m_internalModel->select();
    endResetModel();
}

Qt::ItemFlags DocumentTypeModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return ReferenceModel::flags(index);
}

QModelIndex DocumentTypeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
            return QModelIndex();
        }

        if (!parent.isValid()) {
            return createIndex(row, column, nullptr);
        }

        return QModelIndex();
}

bool DocumentTypeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count -1);
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    endInsertRows();

    if(m_internalModel->insertRows(row, count, internalParent)) {
        m_internalModel->submit();

        return true;
    }

    return false;
}

QVariant DocumentTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_internalModel->headerData(section, orientation, role);
}

QModelIndex DocumentTypeModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool DocumentTypeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count -1);
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    endRemoveRows();

    if(m_internalModel->removeRows(row, count, internalParent)) {
        select();
        return true;
    }

    return false;
}

int DocumentTypeModel::rowCount(const QModelIndex &parent) const
{
    QModelIndex internalParent = m_internalModel->index(parent.row(), parent.column());
    return m_internalModel->rowCount(internalParent);
}

int DocumentTypeModel::columnCount(const QModelIndex &index) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->columnCount(internalIndex);
}

QVariant DocumentTypeModel::data(const QModelIndex &index, int role) const
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());

    if(role == Qt::DecorationRole && index.column() == 1){
        return ColorRect::pixmap(QColor(internalIndex.siblingAtColumn(2).data().toString()), 16);
    }

    return m_internalModel->data(internalIndex, role);
}

bool DocumentTypeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex internalIndex = m_internalModel->index(index.row(), index.column());
    return m_internalModel->setData(internalIndex, value, role);
}

bool DocumentTypeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return m_internalModel->setHeaderData(section, orientation, value, role);
}

void DocumentTypeModel::setDefaults(int, QSqlRecord &record)
{
    record.setValue("name", tr("Document type %1").arg(itemMaxNum(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated("name", true);
}
