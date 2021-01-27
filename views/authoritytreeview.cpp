#include "authoritytreeview.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

AuthorityTreeView::AuthorityTreeView(QWidget *parent) : TreeView(parent)
{
    m_authorityModel = new AuthorityModel;
    m_authorityProxyModel = new AuthorityProxyModel;

    openInNTAction = new QAction(tr("Open in new tab"));
    detailsAction = new QAction(tr("Details"));

    initialize();
    restoreViewState();
}

AuthorityTreeView::~AuthorityTreeView()
{
    saveViewState();

    delete openInNTAction;
    delete detailsAction;

    delete m_authorityModel;
    delete m_authorityProxyModel;
}

void AuthorityTreeView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/AuthorityTreeView").toByteArray());
}

void AuthorityTreeView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("AuthorityTreeView", header()->saveState());
    settings->endGroup();
}

void AuthorityTreeView::initialize()
{
    m_authorityModel->select();
    m_authorityProxyModel->setSourceModel(m_authorityModel);
    m_authorityProxyModel->setDynamicSortFilter(true);

    setModel(m_authorityProxyModel);

    setFrameShape(QFrame::NoFrame);
    hideColumn(1);
    hideColumn(2);
    expandAll();

    connect(openInNTAction, &QAction::triggered, this, [=] {
            emit openInNewTabRequested(currentIndex());
    });
    connect(this, &QTreeView::doubleClicked, this, [=](const QModelIndex &index) {
        emit openInNewTabRequested(index);
    });

    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &AuthorityTreeView::onCurrentChanged);
    connect(detailsAction, &QAction::triggered, this, &AuthorityTreeView::details);

}

void AuthorityTreeView::contextMenu(BaseContextMenu &menu)
{
    QModelIndex index = currentIndex();

    openInNTAction->setEnabled(index.isValid() && index.parent().isValid());
    menu.insertAction(menu.action(BaseContextMenu::Insert), openInNTAction);

    menu.insertSeparator(menu.action(BaseContextMenu::Insert));

    detailsAction->setEnabled(index.isValid() && index.parent().isValid());
    menu.addAction(detailsAction);
}

void AuthorityTreeView::onCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    setInsertEnabled(!current.isValid() || current == root);
    setEditEnabled(current.isValid() && current !=root);
    setRemoveEnabled(current.isValid() && current != root);
    setRefreshEnabled(true);

    application->mainWindow()->action_record->setEnabled(current.isValid() && current != root);

    // set authority id
    m_authorityId = current.siblingAtColumn(0).data(Qt::UserRole);
}

void AuthorityTreeView::details()
{
    QModelIndex v = currentIndex();
    QVariant id = v.siblingAtColumn(2).data(); //id

    AuthorityDetailsDialog dialog(id);
    int res = dialog.exec();

    if(res == AuthorityDetailsDialog::Accepted) {
        m_authorityProxyModel->setData(v, dialog.getName());
    }
}

void AuthorityTreeView::insertRow(const QModelIndex &)
{
    QModelIndex sourceRoot = m_authorityModel->rootItem();
    QModelIndex proxyRoot = m_authorityProxyModel->mapFromSource(sourceRoot);

    if(!isExpanded(proxyRoot)) {
        expand(proxyRoot);
    }

    int v = m_authorityProxyModel->sourceModel()->rowCount(sourceRoot) - 1;

    bool insert = m_authorityProxyModel->sourceModel()->insertRow(v, sourceRoot);

    if(insert) {
        v += 1;
        QModelIndex currentIndex = m_authorityProxyModel->mapFromSource(m_authorityProxyModel->sourceModel()->index(v, 0, sourceRoot));

        setCurrentIndex(currentIndex);
        scrollTo(currentIndex);
        editRow(currentIndex);
    } else {
        QMessageBox::warning(this,
                tr("Public authorities"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AuthorityTreeView::removeRow(const QModelIndex &index)
{
    QModelIndex parent = m_authorityProxyModel->parent(index);

    int res = QMessageBox::critical(this,
        tr("Public authorities"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_authorityProxyModel->removeRow(index.row(), parent);
        if (remove) {
            selectionModel()->clearCurrentIndex();
            selectionModel()->clearSelection();
        } else {
            QMessageBox::warning(this,
                    tr("Public authorities"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}

void AuthorityTreeView::refresh()
{
    m_authorityModel->select();
    expandAll();
}

