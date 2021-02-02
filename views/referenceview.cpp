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


void ReferenceView::insertRow()
{
    bool insert = m_proxyModel->sourceModel()->insertRow(0);

    if(insert) {
        m_proxyModel->submit();
        for (int i = 0; i < m_proxyModel->columnCount(); ++i) {
            resizeColumnToContents(i);
        }

        QModelIndex topLeft = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, 0));
        QModelIndex bottomRight = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, m_proxyModel->columnCount() - 1));

        QItemSelection selection(topLeft, bottomRight);
        selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
        scrollTo(topLeft);

        for (int i=0; i < m_proxyModel->columnCount(); ++i) {
            if(isColumnHidden(i) == false) {
                QModelIndex section = m_proxyModel->index(topLeft.row(), i);
                setCurrentIndex(section);
                edit(section);
                break;
            }
        }
    } else {
        QMessageBox::warning(this,
                tr("References"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void ReferenceView::removeRows()
{
    int res = QMessageBox::critical(this,
        tr("References"),
        tr("Are you shure that you want to delete this items?"),
            QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        const QModelIndexList &indexes = selectionModel()->selectedRows();

        for (int i = 0; i < indexes.count(); ++i) {
            const QModelIndex index = indexes.at(i);
            QModelIndex parent = m_proxyModel->parent(index);

            bool remove = m_proxyModel->removeRow(index.row(), parent);
            if (remove) {
                clearSelection();
                refresh();
            } else {
                QMessageBox::warning(this,
                        tr("References"),
                        tr("Could not remove the items."),
                        QMessageBox::Ok);
            }
        }
    }
}

void ReferenceView::refresh()
{
    if(m_proxyModel != nullptr) {
        QSqlTableModel *model = dynamic_cast<QSqlTableModel*>(m_proxyModel->sourceModel());

        if(model)
            model->select();
    }
}

