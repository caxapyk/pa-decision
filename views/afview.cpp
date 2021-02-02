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
    if(selectionModel()->selection().isEmpty()) {
        menu.removeAction(menu.action(BaseContextMenu::Insert));
        menu.removeAction(menu.action(BaseContextMenu::Edit));
        menu.removeAction(menu.action(BaseContextMenu::Remove));
    }
}

void AFView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!selected.isEmpty()) {
        const QModelIndex current = selected.indexes().last();
        AFTreeModel::RecordNode *node = static_cast<AFTreeModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());
        setInsertEnabled(node->level != AFTreeModel::RecordLevel);
    } else {
        setInsertEnabled(false);
    }

    setEditEnabled(!selected.isEmpty());
    setRemoveEnabled(!selected.isEmpty());
    setRefreshEnabled(true);

    QTreeView::selectionChanged(selected, deselected);
    emit _selected(selected, deselected);
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

void AFView::insertRow()
{
    QModelIndex proxyParent = currentIndex();
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
                tr("Archive fund"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AFView::removeRows()
{
    int res = QMessageBox::critical(this,
        tr("Archive fund"),
        tr("Are you shure that you want to delete this item(s)?"),
            QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        const QModelIndexList &indexes = selectionModel()->selectedRows();

        for (int i = 0; i < indexes.count(); ++i) {
            const QModelIndex index = indexes.at(i);
            QModelIndex parent = m_proxyModel->parent(index);

            bool remove = m_proxyModel->removeRow(index.row(), parent);
            if (remove) {
                //
            } else {
                QMessageBox::warning(this,
                tr("Archive fund"),
                tr("Could not remove the items."),
                    QMessageBox::Ok);
            }
        }

        clearSelection();
    }
}

void AFView::refresh()
{
    AFTreeModel *model = dynamic_cast<AFTreeModel*>(m_proxyModel->sourceModel());
    if(model)
        model->select();

    emit _selected(QItemSelection(), QItemSelection());
}
