#include "authorityview.h"
#include "ui_authorityview.h"

#include "application.h"
#include "dialogs/authoritydetaildialog.h"

#include <QDebug>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>

AuthorityView::AuthorityView(QWidget *parent) :
    View(parent),
    ui(new Ui::AuthorityView)
{
    ui->setupUi(this);

    initialize();
}

AuthorityView::~AuthorityView()
{
    delete ui;

    delete m_authorityModel;
    delete m_authorityProxyModel;
    delete openShortcut;
    delete refreshShortcut;
    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
}

void AuthorityView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->tV_authority->header()->restoreState(settings->value("Views/tV_authority").toByteArray());
}

void AuthorityView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("tV_authority", ui->tV_authority->header()->saveState());
    settings->endGroup();
}

void AuthorityView::initialize()
{
    m_authorityModel = new AuthorityTreeModel;
    m_authorityModel->select();

    m_authorityProxyModel = new AuthorityProxyModel;
    m_authorityProxyModel->setSourceModel(m_authorityModel);

    ui->tV_authority->setModel(m_authorityProxyModel);
    ui->tV_authority->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tV_authority->expandAll();

    setupShortcuts();

    connect(ui->tV_authority, &QMenu::customContextMenuRequested, this, &AuthorityView::contextMenu);
    connect(ui->tV_authority->selectionModel(), &QItemSelectionModel::currentChanged, this, &AuthorityView::changeCurrent);
    // change current to root on collapse tree
    connect(ui->tV_authority, &QTreeView::collapsed, this, [=](const QModelIndex &index){
        changeCurrent(index, QModelIndex());
    });

    ui->tV_authority->setCurrentIndex(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
}

void AuthorityView::setupShortcuts()
{
    openShortcut = new QShortcut(QKeySequence::Open, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    connect(openShortcut, &QShortcut::activated, this, &AuthorityView::open);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &AuthorityView::refresh);

    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &AuthorityView::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &AuthorityView::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &AuthorityView::remove);
}

void AuthorityView::changeCurrent(const QModelIndex &current, const QModelIndex &)
{
    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    openShortcut->setEnabled((current.isValid() && (current != root || (current == root && !ui->tV_authority->isExpanded(current)))));
    refreshShortcut->setEnabled(true);
    insertShortcut->setEnabled(current.isValid() && current == root);
    editShortcut->setEnabled(current.isValid() && current !=root);
    removeShortcut->setEnabled(current.isValid() && current != root);
}

void AuthorityView::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_authority->indexAt(ui->tV_authority->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_authority->setCurrentIndex(currentIndex);

    QMenu menu;

    QAction openAction(QIcon(":/icons/icons/open-16.png"), tr("Open"));
    openAction.setShortcut(openShortcut->key());
    openAction.setEnabled(openShortcut->isEnabled());
    connect(&openAction, &QAction::triggered, this, &AuthorityView::open);
    menu.addAction(&openAction);

    QAction refreshAction(QIcon(":/icons/icons/refresh-16.png"), tr("Refresh"));
    refreshAction.setShortcut(refreshShortcut->key());
    refreshAction.setEnabled(refreshShortcut->isEnabled());
    connect(&refreshAction, &QAction::triggered, this, &AuthorityView::refresh);
    menu.addAction(&refreshAction);

    menu.addSeparator();

    QAction insertAction(QIcon(":/icons/icons/add-16.png"), tr("New"));
    insertAction.setShortcut(insertShortcut->key());
    insertAction.setEnabled(insertShortcut->isEnabled());
    connect(&insertAction, &QAction::triggered, this, &AuthorityView::insert);
    menu.addAction(&insertAction);

    QAction editAction(QIcon(":/icons/icons/edit-16.png"), tr("Edit"));
    editAction.setShortcut(editShortcut->key());
    editAction.setEnabled(editShortcut->isEnabled());
    connect(&editAction, &QAction::triggered, this,  &AuthorityView::edit);
    menu.addAction(&editAction);

    QAction removeAction(QIcon(":/icons/icons/remove-16.png"), tr("Remove"));
    removeAction.setShortcut(removeShortcut->key());
    removeAction.setEnabled(removeShortcut->isEnabled());
    connect(&removeAction, &QAction::triggered, this,  &AuthorityView::remove);
    menu.addAction(&removeAction);

    menu.addSeparator();

    QAction infoAction(tr("Detail"));
    infoAction.setEnabled(currentIndex.isValid() && ui->tV_authority->currentIndex() != m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    connect(&infoAction, &QAction::triggered, this,  &AuthorityView::info);
    menu.addAction(&infoAction);

    menu.exec(QCursor().pos());
}

void AuthorityView::open()
{
    QModelIndex index = ui->tV_authority->currentIndex();
    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    if(index == root) {
        ui->tV_authority->expand(root);
    } else {
        // load data
    }
}

void AuthorityView::refresh()
{
    m_authorityProxyModel->invalidate();
    m_authorityModel->select();

    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    ui->tV_authority->expandAll();
    ui->tV_authority->setCurrentIndex(root);
}

void AuthorityView::insert()
{
    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    if(!ui->tV_authority->isExpanded(root)) {
        ui->tV_authority->expand(root);
    }

    int v = 0;

    if(m_authorityProxyModel->sortOrder() == Qt::DescendingOrder && m_authorityProxyModel->rowCount(root) > 0)
        v = m_authorityProxyModel->rowCount(root) - 1;

    bool insert = m_authorityProxyModel->insertRows(v, 1, root);

    if(insert) {
        if(m_authorityProxyModel->sortOrder() == Qt::DescendingOrder)
            v += 1;

        QModelIndex currentIndex = m_authorityProxyModel->index(v, 0, root);

        ui->tV_authority->setCurrentIndex(currentIndex);
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
    QModelIndex index = ui->tV_authority->currentIndex();

    ui->tV_authority->edit(index);
}

void AuthorityView::remove()
{
    QModelIndex index = ui->tV_authority->currentIndex();
    QModelIndex root = m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem());

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_authorityProxyModel->removeRow(index.row(), root);
        if (remove) {
            QMessageBox::information(this,
                    tr("Deleting item"),
                    tr("Item has been successfully deleted."),
                    QMessageBox::Ok);
        } else {
            QMessageBox::warning(this,
                    tr("Deleting item"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}

void AuthorityView::info()
{
    QModelIndex index = ui->tV_authority->currentIndex();

    AuthorityDetailDialog dialog(index.data(Qt::UserRole));
    int res = dialog.exec();

    if(res == AuthorityDetailDialog::Accepted) {
        refresh();
    }
}
