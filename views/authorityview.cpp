#include "authorityview.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

AuthorityView::AuthorityView(QWidget *parent) : TreeView(parent)
{
    m_model = new AuthorityModel;
    m_proxyModel = new AuthoritySortModel;

    openInNTAction = new QAction(tr("Open in new tab"));
    detailsAction = new QAction(tr("Details"));

    initialize();
    restoreViewState();
}

AuthorityView::~AuthorityView()
{
    saveViewState();

    delete openInNTAction;
    delete detailsAction;

    delete m_model;
    delete m_proxyModel;
}

void AuthorityView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/AuthorityView").toByteArray());
}

void AuthorityView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("AuthorityView", header()->saveState());
    settings->endGroup();
}

void AuthorityView::initialize()
{
    m_model->select();
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setDynamicSortFilter(true);

    setModel(m_proxyModel);

    setFrameShape(QFrame::NoFrame);
    hideColumn(1);
    hideColumn(2);
    expandAll();

    connect(openInNTAction, &QAction::triggered, this, [=] {
        QModelIndex v = currentIndex();
        emit openInNewTabRequested(v.siblingAtColumn(0).data(Qt::UserRole));
    });
    connect(this, &QTreeView::doubleClicked, this, [=]() {
        QModelIndex v = currentIndex();
        emit openInNewTabRequested(v.siblingAtColumn(0).data(Qt::UserRole));
    });

    connect(detailsAction, &QAction::triggered, this, &AuthorityView::details);
}

void AuthorityView::contextMenu(BaseContextMenu &menu)
{
    QModelIndex index = currentIndex();

    openInNTAction->setEnabled(index.isValid() && index.parent().isValid());
    menu.insertAction(menu.action(BaseContextMenu::Insert), openInNTAction);

    menu.insertSeparator(menu.action(BaseContextMenu::Insert));

    detailsAction->setEnabled(index.isValid() && index.parent().isValid());
    menu.addAction(detailsAction);
}

void AuthorityView::selectionChanged(const QItemSelection &selected, const QItemSelection &diselected)
{
    QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    setInsertEnabled(selected.isEmpty() || selected.contains(root));
    setEditEnabled(!selected.isEmpty() && !selected.contains(root));
    setRemoveEnabled(!selected.isEmpty() && !selected.contains(root));
    setRefreshEnabled(true);

    //application->mainWindow()->action_record->setEnabled(current.isValid() && current != root);

    // set authority id
    if(!selected.isEmpty())
        m_authorityId = selected.indexes().at(0).siblingAtColumn(0).data(Qt::UserRole);

    QTreeView::selectionChanged(selected, diselected);
}

void AuthorityView::details()
{
    const QModelIndex v = currentIndex();
    QVariant id = v.siblingAtColumn(2).data(); //id

    AuthorityDetailsDialog dialog(id);
    int res = dialog.exec();

    if(res == AuthorityDetailsDialog::Accepted) {
        m_proxyModel->setData(v, dialog.getName());
    }
}

void AuthorityView::insertRow()
{
    QModelIndex sourceRoot = m_model->rootItem();
    QModelIndex proxyRoot = m_proxyModel->mapFromSource(sourceRoot);

    if(!isExpanded(proxyRoot)) {
        expand(proxyRoot);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceRoot) - 1;

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceRoot);

    if(insert) {
        v += 1;
        QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceRoot));

        setCurrentIndex(currentIndex);
        scrollTo(currentIndex);
        edit(currentIndex);
    } else {
        QMessageBox::warning(this,
                tr("Public authorities"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AuthorityView::removeRows()
{
    int res = QMessageBox::critical(this,
        tr("Public authorities"),
        tr("Are you shure that you want to delete this item(s)?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        const QModelIndexList &indexes = selectionModel()->selectedRows();

        for (int i = 0; i < indexes.count(); ++i) {
            const QModelIndex index = indexes.at(i);
            QModelIndex parent = m_proxyModel->parent(index);

            bool remove = m_proxyModel->removeRow(index.row(), parent);
            if (remove) {
                selectionModel()->clearCurrentIndex();
                selectionModel()->clearSelection();
            } else {
                QMessageBox::warning(this,
                        tr("Public authorities"),
                        tr("Could not remove the items."),
                        QMessageBox::Ok);
            }
        }
    }
}

void AuthorityView::refresh()
{
    m_model->select();
}

