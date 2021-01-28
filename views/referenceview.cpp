#include "referenceview.h"

#include <QDebug>
#include <QMessageBox>

ReferenceView::ReferenceView(QWidget *parent) : TreeView(parent)
{
    m_proxyModel = new QSortFilterProxyModel;

    restoreViewState();
}

ReferenceView::~ReferenceView()
{
    delete m_proxyModel;
}

void ReferenceView::_setModel(QSqlTableModel *model)
{
    m_proxyModel->setSourceModel(model);
    setModel(m_proxyModel);

    hideColumn(0); // id column
}


void ReferenceView::insertRow(const QModelIndex &)
{
    bool insert = m_proxyModel->sourceModel()->insertRow(0);

    if(insert) {
        m_proxyModel->submit();
        for (int i = 0; i < m_proxyModel->columnCount(); ++i) {
            resizeColumnToContents(i);
        }

        selectionModel()->clearCurrentIndex();

        QModelIndex topLeft = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, 0));
        QModelIndex bottomRight = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, m_proxyModel->columnCount() - 1));

        QItemSelection selection(topLeft, bottomRight);
        selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
        setCurrentIndex(topLeft);
        scrollTo(topLeft);

        for (int i=0; i < m_proxyModel->columnCount(); ++i) {
            if(isColumnHidden(i) == false) {
                QModelIndex section = m_proxyModel->index(topLeft.row(), i);
                edit(section);
            }
        }
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void ReferenceView::removeRow(const QModelIndex &index)
{
    if(m_proxyModel != nullptr) {
        QModelIndex parent = m_proxyModel->parent(index);

        int res = QMessageBox::critical(this,
            tr("Deleting item"),
            tr("Are you shure that you want to delete this item?"),
            QMessageBox::No | QMessageBox::Yes);

        if (res == QMessageBox::Yes) {
            bool remove = m_proxyModel->removeRow(index.row(), parent);
            if (remove) {
                refresh();
            } else {
                QMessageBox::warning(this,
                        tr("Deleting item"),
                        tr("Could not remove the item."),
                        QMessageBox::Ok);
            }
        }
    }
}

void ReferenceView::refresh()
{
    if(m_proxyModel != nullptr) {
        //m_proxyModel->invalidate();
        //m_tree->sortByColumn(-1, Qt::AscendingOrder);

        QSqlTableModel *model = dynamic_cast<QSqlTableModel*>(m_proxyModel->sourceModel());

        if(model)
            model->select();
    }
}

