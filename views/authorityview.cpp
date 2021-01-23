#include "authorityview.h"
#include "ui_authorityview.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"
#include "widgets/decisiontab.h"

#include <QDebug>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QMessageBox>

AuthorityView::AuthorityView(QWidget *parent) :
    View(parent),
    ui(new Ui::AuthorityView)
{
    ui->setupUi(this);

    m_tree = new CustomTreeView;
    ui->tab_authorities->layout()->addWidget(m_tree);

    initialize();
    restoreViewState();
}

AuthorityView::~AuthorityView()
{
    saveViewState();

    delete ui;
    delete m_tree;

    delete m_authorityModel;
    delete m_authorityProxyModel;
}

void AuthorityView::initialize()
{
    m_authorityModel = new AuthorityModel;
    m_authorityModel->select();

    m_authorityProxyModel = new AuthorityProxyModel;
    m_authorityProxyModel->setSourceModel(m_authorityModel);
    m_authorityProxyModel->setDynamicSortFilter(true);
    m_tree->setModel(m_authorityProxyModel);

    m_tree->setFrameShape(QFrame::NoFrame);

    m_tree->hideColumn(1);
    m_tree->hideColumn(2);
    m_tree->expandAll();

    connect(m_tree, &CustomTreeView::onContextMenuRequested, this, &AuthorityView::contextMenu);
    connect(m_tree, &QTreeView::doubleClicked, this, &AuthorityView::openInNewTab);
    connect(m_tree->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AuthorityView::selected);

    connect(m_tree, &CustomTreeView::onInsert, this, &AuthorityView::insert);
    connect(m_tree, &CustomTreeView::onEdit, this,  &AuthorityView::edit);
    connect(m_tree, &CustomTreeView::onRemove, this,  &AuthorityView::remove);
    connect(m_tree, &CustomTreeView::onRefresh, this, &AuthorityView::refresh);
}

void AuthorityView::restoreViewState()
{

}

void AuthorityView::saveViewState()
{

}

void AuthorityView::contextMenu(CustomContextMenu &menu)
{
    QModelIndex currentIndex = m_tree->indexAt(m_tree->viewport()->mapFromGlobal(QCursor().pos()));
    m_tree->setCurrentIndex(currentIndex);

    QAction openInNTAction(tr("Open in new tab"));
    connect(&openInNTAction, &QAction::triggered, this, [=] { openInNewTab(currentIndex); });
    openInNTAction.setEnabled(currentIndex.isValid() && currentIndex.parent().isValid());
    menu.insertAction(menu.action(CustomContextMenu::Insert), &openInNTAction);

    menu.insertSeparator(menu.action(CustomContextMenu::Insert));

    QAction detailsAction(tr("Details"));
    connect(&detailsAction, &QAction::triggered, this,  &AuthorityView::details);
    detailsAction.setEnabled(currentIndex.isValid() && currentIndex.parent().isValid());
    menu.addAction(&detailsAction);

    menu.exec(QCursor().pos());
}

void AuthorityView::selected(const QItemSelection &selected, const QItemSelection &)
{
    QModelIndex current = !selected.isEmpty() ? selected.indexes().at(0) : QModelIndex();
    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    m_tree->setInsertEnabled(selected.isEmpty() || current == root);
    m_tree->setEditEnabled(!selected.isEmpty() && current !=root);
    m_tree->setRemoveEnabled(!selected.isEmpty() && current != root);
    m_tree->setRefreshEnabled(true);

    application->mainWindow()->action_record->setEnabled(!selected.isEmpty() && current != root);

    // set authority id
    m_authorityId = current.siblingAtColumn(0).data(Qt::UserRole);
}

void AuthorityView::openInNewTab(const QModelIndex &index)
{
    if(index.isValid() && index.parent().isValid()) {
        DecisionTab *tab = new DecisionTab;
        tab->view()->table()->setAuthorityId(index.data(Qt::UserRole));
        tab->view()->table()->refresh();

        QIcon icon(index.data(Qt::DecorationRole).toString());

        explorer()->createTab(tab, index.data().toString(), icon);
    }
}

DecisionView* AuthorityView::currentDecisionView()
{
    DecisionTab *tab = dynamic_cast<DecisionTab*>(explorer()->currentTab());
    return tab->view();
}

void AuthorityView::insert()
{
    QModelIndex sourceRoot = m_authorityModel->rootItem();
    QModelIndex proxyRoot = m_authorityProxyModel->mapFromSource(sourceRoot);

    if(!m_tree->isExpanded(proxyRoot)) {
        m_tree->expand(proxyRoot);
    }

    int v = m_authorityProxyModel->sourceModel()->rowCount(sourceRoot) - 1;

    bool insert = m_authorityProxyModel->sourceModel()->insertRow(v, sourceRoot);

    if(insert) {
        v += 1;
        QModelIndex currentIndex = m_authorityProxyModel->mapFromSource(m_authorityProxyModel->sourceModel()->index(v, 0, sourceRoot));

        m_tree->setCurrentIndex(currentIndex);
        m_tree->scrollTo(currentIndex);
        m_tree->edit(currentIndex);
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AuthorityView::edit()
{
    m_tree->edit(m_tree->currentIndex());
}

void AuthorityView::details()
{
    QModelIndex index = m_tree->currentIndex();
    QVariant id = index.siblingAtColumn(2).data(); //id

    AuthorityDetailsDialog dialog(id);
    int res = dialog.exec();

    if(res == AuthorityDetailsDialog::Accepted) {
        m_authorityProxyModel->setData(index, dialog.getName());
    }
}

void AuthorityView::remove()
{
    QModelIndex index = m_tree->currentIndex();
    QModelIndex parent = m_authorityProxyModel->parent(index);

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_authorityProxyModel->removeRow(index.row(), parent);
        if (remove) {
            m_tree->setCurrentIndex(QModelIndex());
        } else {
            QMessageBox::warning(this,
                    tr("Deleting item"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}

void AuthorityView::refresh()
{
    m_authorityModel->select();
    m_tree->expandAll();
    selected(QItemSelection(), QItemSelection());
}

void AuthorityView::openIndexTab() {
    // index tab page
}
