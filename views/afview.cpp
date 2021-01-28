#include "afview.h"
#include "application.h"
#include "dialogs/protocoldetailsdialog.h"
#include "dialogs/funddetailsdialog.h"
#include "dialogs/inventorydetailsdialog.h"
#include "dialogs/recorddetailsdialog.h"
#include "dialogs/protocoldialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

AFView::AFView(QWidget *parent) : TreeView(parent)
{
    m_model = new AFTreeModel;
    m_proxyModel = new AFSortModel;

    restoreViewState();

    m_proxyModel->setSourceModel(m_model);
    setModel(m_proxyModel);

    hideColumn(1);
    hideColumn(2);
    hideColumn(3);

    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &AFView::currentChanged);
}

AFView::~AFView()
{
    saveViewState();

    delete m_model;
    delete m_proxyModel;
}

void AFView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/AFView").toByteArray());
}

void AFView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("AFView", header()->saveState());
    settings->endGroup();
}

void AFView::contextMenu(BaseContextMenu &menu)
{
    QModelIndex index = indexAt(viewport()->mapFromGlobal(QCursor().pos()));

    if(!index.isValid()) {
        menu.action(BaseContextMenu::Edit)->setDisabled(true);
        menu.action(BaseContextMenu::Remove)->setDisabled(true);
    }

    setCurrentIndex(index);
}

void AFView::currentChanged(const QModelIndex &current, const QModelIndex &)
{
    AFTreeModel::RecordNode *node = static_cast<AFTreeModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    setInsertEnabled(node == nullptr || node->level != AFTreeModel::RecordLevel);
    setEditEnabled(current.isValid());
    setRemoveEnabled(current.isValid());
    setRefreshEnabled(true);
}

void AFView::details()
{
    QModelIndex index = currentIndex();
    AFTreeModel::RecordNode *node = static_cast<AFTreeModel::RecordNode*>(m_proxyModel->mapToSource(index).internalPointer());

    switch (node->level) {
    case AFTreeModel::FundLevel:
    {
        QVariant id = index.siblingAtColumn(2).data(); //id

        FundDetailsDialog dialog(id);
        int res = dialog.exec();

        if(res == FundDetailsDialog::Accepted) {
            m_proxyModel->setData(index.siblingAtColumn(0), dialog.getNumber());
            m_proxyModel->setData(index.siblingAtColumn(3),dialog.getName());
        }
    }
        break;
    case AFTreeModel::InventoryLevel:
    {
        QVariant id = index.siblingAtColumn(2).data(); //id

        InventoryDetailsDialog dialog(id);
        int res = dialog.exec();

        if(res == InventoryDetailsDialog::Accepted) {
            m_proxyModel->setData(index.siblingAtColumn(0), dialog.getNumber());
            m_proxyModel->setData(index.siblingAtColumn(3),dialog.getName());
        }
    }
        break;
    case AFTreeModel::RecordLevel:
    {
        QVariant id = index.siblingAtColumn(2).data(); //id

        RecordDetailsDialog dialog(id);
        int res = dialog.exec();

        if(res == RecordDetailsDialog::Accepted) {
            m_proxyModel->setData(index.siblingAtColumn(0), dialog.getNumber());
            m_proxyModel->setData(index.siblingAtColumn(3),dialog.getName());
        }
    }
        break;
    }
}

void AFView::protocols()
{
    QModelIndex index = currentIndex();
    QVariant id = index.data(Qt::UserRole); //id

    ProtocolDialog dialog;

    ProtocolView *view = dynamic_cast<ProtocolView*>(dialog.treeView());
    view->model()->setRecordId(id);
    dialog.exec();
}

void AFView::insertRow(const QModelIndex &index)
{
    QModelIndex proxyParent = index;
    QModelIndex sourceParent = m_proxyModel->mapToSource(proxyParent);

    if(!isExpanded(proxyParent)) {
        expand(proxyParent);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceParent);

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceParent);

    if(insert) {
        QModelIndex current = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceParent));

        setCurrentIndex(current);
        scrollTo(current);
        edit(currentIndex());
    } else {
        QMessageBox::warning(this,
                tr("Archive records"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AFView::removeRow(const QModelIndex &index)
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
                //
            } else {
                QMessageBox::warning(this,
                        tr("Deleting item"),
                        tr("Could not remove the item."),
                        QMessageBox::Ok);
            }
        }
    }
}

void AFView::refresh()
{
    AFTreeModel *model = dynamic_cast<AFTreeModel*>(m_proxyModel->sourceModel());
    if(model)
        model->select();
}
