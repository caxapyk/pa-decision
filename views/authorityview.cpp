#include "authorityview.h"
#include "ui_authorityview.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"
#include "widgets/customcontextmenu.h"
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
    initialize();

    restoreViewState();
}

AuthorityView::~AuthorityView()
{
    saveViewState();

    delete ui;

    delete m_authorityModel;
    delete m_authorityProxyModel;
}

void AuthorityView::initialize()
{
    m_authorityModel = new AuthorityModel;
    m_authorityModel->select();

    m_authorityProxyModel = new AuthorityProxyModel;
    m_authorityProxyModel->setSourceModel(m_authorityModel);
    m_authorityProxyModel->setDynamicSortFilter(false);
    ui->tV_authority->setModel(m_authorityProxyModel);

    ui->tV_authority->hideColumn(1);
    ui->tV_authority->hideColumn(2);
    ui->tV_authority->expandAll();

    m_authorityProxyModel->sort(0, Qt::AscendingOrder);

    ui->tV_authority->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tV_authority, &QMenu::customContextMenuRequested, this, &AuthorityView::contextMenu);

    connect(ui->tV_authority, &QTreeView::doubleClicked, this, &AuthorityView::openInNewTab);
    connect(ui->tV_authority->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AuthorityView::selected);

    setupShortcuts();
}

void AuthorityView::restoreViewState()
{

}

void AuthorityView::saveViewState()
{

}

void AuthorityView::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    insertShortcut->setEnabled(true);
    connect(insertShortcut, &QShortcut::activated, this, &AuthorityView::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    editShortcut->setEnabled(false);
    connect(editShortcut, &QShortcut::activated, this, &AuthorityView::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    removeShortcut->setEnabled(false);
    connect(removeShortcut, &QShortcut::activated, this, &AuthorityView::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    refreshShortcut->setEnabled(true);
    connect(refreshShortcut, &QShortcut::activated, this, &AuthorityView::refresh);
}

void AuthorityView::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_authority->indexAt(ui->tV_authority->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_authority->setCurrentIndex(currentIndex);

    CustomContextMenu menu(CustomContextMenu::Insert | CustomContextMenu::Edit | CustomContextMenu::Remove | CustomContextMenu::Refresh);

    QAction openInNTAction(tr("Open in new tab"));
    connect(&openInNTAction, &QAction::triggered, this, [=] {
        openInNewTab(currentIndex);
    });
    openInNTAction.setEnabled(currentIndex.isValid() && currentIndex.parent().isValid());
    menu.insertAction(menu.action(CustomContextMenu::Insert), &openInNTAction);

    menu.insertSeparator(menu.action(CustomContextMenu::Insert));

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, &AuthorityView::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &AuthorityView::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &AuthorityView::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, &AuthorityView::refresh);

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

    refreshShortcut->setEnabled(true);
    insertShortcut->setEnabled(selected.isEmpty() || current == root);
    editShortcut->setEnabled(!selected.isEmpty() && current !=root);
    removeShortcut->setEnabled(!selected.isEmpty() && current != root);

    application->mainWindow()->action_record->setEnabled(!selected.isEmpty() && current != root);

    // set authority id
    m_authorityId = current.siblingAtColumn(0).data(Qt::UserRole);
}

void AuthorityView::openInNewTab(const QModelIndex &index)
{
    if(index.isValid() && index.parent().isValid()) {
        DecisionTab *tab = new DecisionTab;
        tab->view()->model()->setAuthorityId(index.data(Qt::UserRole).toInt());
        tab->view()->model()->select();

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

    if(!ui->tV_authority->isExpanded(proxyRoot)) {
        ui->tV_authority->expand(proxyRoot);
    }

    int v = m_authorityProxyModel->sourceModel()->rowCount(sourceRoot) - 1;

    bool insert = m_authorityProxyModel->sourceModel()->insertRow(v, sourceRoot);

    if(insert) {
        v += 1;
        QModelIndex currentIndex = m_authorityProxyModel->mapFromSource(m_authorityProxyModel->sourceModel()->index(v, 0, sourceRoot));

        ui->tV_authority->setCurrentIndex(currentIndex);
        ui->tV_authority->scrollTo(currentIndex);
        ui->tV_authority->edit(currentIndex);
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AuthorityView::edit()
{
    ui->tV_authority->edit(ui->tV_authority->currentIndex());
}

void AuthorityView::details()
{
    QModelIndex index = ui->tV_authority->currentIndex();
    QVariant id = index.siblingAtColumn(2).data(); //id

    AuthorityDetailsDialog dialog(id);
    int res = dialog.exec();

    if(res == AuthorityDetailsDialog::Accepted) {
        m_authorityProxyModel->setData(index, dialog.getName());
    }
}

void AuthorityView::remove()
{
    QModelIndex index = ui->tV_authority->currentIndex();
    QModelIndex parent = m_authorityProxyModel->parent(index);

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_authorityProxyModel->removeRow(index.row(), parent);
        if (remove) {
            ui->tV_authority->setCurrentIndex(QModelIndex());
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
    ui->tV_authority->expandAll();
    selected(QItemSelection(), QItemSelection());
}

void AuthorityView::openIndexTab() {
    // index tab page
}
