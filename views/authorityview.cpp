#include "authorityview.h"
#include "ui_authorityview.h"

#include "application.h"
#include "dialogs/authoritydetaildialog.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>

AuthorityView::AuthorityView(QWidget *parent) :
    View(parent),
    ui(new Ui::AuthorityView)
{
    ui->setupUi(this);

    initialize();
    setupShortcuts();
}

AuthorityView::~AuthorityView()
{
    delete ui;

    delete m_authorityModel;
    delete m_recordModel;
    delete m_recordProxyModel;

    delete openShortcut;
    delete refreshShortcut;
    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
}

void AuthorityView::initialize()
{
    m_authorityModel = new AuthorityTreeModel();
    m_authorityModel->select();
    ui->tV_tree->setModel(m_authorityModel);
    ui->tV_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tV_tree->expandAll();
    ui->tV_tree->setCurrentIndex(m_authorityModel->rootItem());

    connect(ui->tV_tree, &QMenu::customContextMenuRequested, this, &AuthorityView::contextMenu);
    connect(ui->tV_tree, &QTreeView::clicked, this, &AuthorityView::clicked);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &AuthorityView::tabSwitch);
}

void AuthorityView::contextMenu()
{
    QModelIndex currentIndex = ui->tV_tree->indexAt(ui->tV_tree->viewport()->mapFromGlobal(QCursor().pos()));
    clicked(currentIndex);

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
    infoAction.setEnabled(currentIndex.isValid() && ui->tV_tree->currentIndex() != m_authorityModel->rootItem());
    connect(&infoAction, &QAction::triggered, this,  &AuthorityView::info);
    menu.addAction(&infoAction);

    menu.exec(QCursor().pos());
}

void AuthorityView::setupShortcuts()
{
    openShortcut = new QShortcut(QKeySequence::Open, ui->tV_tree, nullptr, nullptr, Qt::WidgetShortcut);
    connect(openShortcut, &QShortcut::activated, this, &AuthorityView::open);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_tree, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &AuthorityView::refresh);

    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_tree, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &AuthorityView::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_tree, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &AuthorityView::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_tree, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &AuthorityView::remove);
}

void AuthorityView::tabSwitch(int index)
{
    switch (index) {
    case AuthorityView::TabAuthority:
        break;
    case AuthorityView::TabCollection:
        loadCollection();
        break;
    case AuthorityView::TabSearch:
        break;

    }
}

void AuthorityView::loadCollection()
{
    switch(ui->cB_collection->currentIndex()){
    case AuthorityView::CollectionRecord:
        if(m_recordModel == nullptr)
            initializeRecord();
        break;
    case AuthorityView::CollectionYear:
        break;
    case AuthorityView::CollectionMember:
        break;
    }
}

void AuthorityView::initializeRecord()
{
    m_recordModel = new RecordTreeModel;
    m_recordModel->select();

    m_recordProxyModel = new QSortFilterProxyModel;
    m_recordProxyModel->setSourceModel(m_recordModel);

    ui->tV_collection->setModel(m_recordProxyModel);
}

void AuthorityView::clicked(const QModelIndex &index)
{
    openShortcut->setEnabled((index.isValid() && (index != m_authorityModel->rootItem() || (index == m_authorityModel->rootItem() && !ui->tV_tree->isExpanded(index)))));
    refreshShortcut->setEnabled(index.parent() != m_authorityModel->rootItem());
    insertShortcut->setEnabled(index.isValid() && index == m_authorityModel->rootItem());
    editShortcut->setEnabled(index.isValid() && index != m_authorityModel->rootItem());
    removeShortcut->setEnabled(index.isValid() && index != m_authorityModel->rootItem());

    ui->tV_tree->setCurrentIndex(index);
}

void AuthorityView::open()
{
    QModelIndex index = ui->tV_tree->currentIndex();

    if(index == m_authorityModel->rootItem()) {
        ui->tV_tree->expand(m_authorityModel->rootItem());
    } else {
        // load data
    }
}

void AuthorityView::refresh()
{
    m_authorityModel->select();
    ui->tV_tree->expandAll();
    ui->tV_tree->setCurrentIndex(m_authorityModel->rootItem());
}

void AuthorityView::insert()
{
    if(!ui->tV_tree->isExpanded(m_authorityModel->rootItem())) {
        ui->tV_tree->expand(m_authorityModel->rootItem());
    }

    bool insert = m_authorityModel->insertRows(
                m_authorityModel->rowCount(m_authorityModel->rootItem()) - 1, 1, m_authorityModel->rootItem());
    if(insert) {
        QModelIndex currentIndex = m_authorityModel->index(
                    m_authorityModel->rowCount(m_authorityModel->rootItem()) -1, 0, m_authorityModel->rootItem());
        ui->tV_tree->setCurrentIndex(currentIndex);
        ui->tV_tree->edit(currentIndex);
    }
}

void AuthorityView::edit()
{
    QModelIndex index = ui->tV_tree->currentIndex();

    ui->tV_tree->edit(index);
}

void AuthorityView::remove()
{
    QModelIndex index = ui->tV_tree->currentIndex();

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_authorityModel->removeRow(index.row(), m_authorityModel->rootItem());
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
    QModelIndex index = ui->tV_tree->currentIndex();

    AuthorityDetailDialog dialog(index.data(Qt::UserRole));
    int res = dialog.exec();

    if(res == AuthorityDetailDialog::Accepted) {
        refresh();
    }
}
