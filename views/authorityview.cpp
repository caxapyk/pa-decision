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

    ui->tV_authority->setCurrentIndex(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));

    connect(ui->tV_authority, &QMenu::customContextMenuRequested, this, &AuthorityView::contextMenu);
    connect(ui->tV_authority, &QTreeView::clicked, this, &AuthorityView::clicked);

    setupShortcuts();
}

void AuthorityView::contextMenu()
{
    QModelIndex currentIndex = ui->tV_authority->indexAt(ui->tV_authority->viewport()->mapFromGlobal(QCursor().pos()));
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
    infoAction.setEnabled(currentIndex.isValid() && ui->tV_authority->currentIndex() != m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    connect(&infoAction, &QAction::triggered, this,  &AuthorityView::info);
    menu.addAction(&infoAction);

    menu.exec(QCursor().pos());
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

void AuthorityView::clicked(const QModelIndex &index)
{
    openShortcut->setEnabled((index.isValid() && (index != m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()) || (index == m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()) && !ui->tV_authority->isExpanded(index)))));
    refreshShortcut->setEnabled(index.parent() != m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    insertShortcut->setEnabled(index.isValid() && index == m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    editShortcut->setEnabled(index.isValid() && index !=m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    removeShortcut->setEnabled(index.isValid() && index != m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));

    ui->tV_authority->setCurrentIndex(index);
}

void AuthorityView::open()
{
    QModelIndex index = ui->tV_authority->currentIndex();

    if(index == m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem())) {
        ui->tV_authority->expand(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    } else {
        // load data
    }
}

void AuthorityView::refresh()
{
    m_authorityProxyModel->invalidate();
    m_authorityModel->select();

    ui->tV_authority->expandAll();
    ui->tV_authority->setCurrentIndex(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
}

void AuthorityView::insert()
{
    if(!ui->tV_authority->isExpanded(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()))) {
        ui->tV_authority->expand(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    }

    bool insert = false;

    if(m_authorityProxyModel->sortOrder() == Qt::AscendingOrder) {
        insert = m_authorityProxyModel->insertRows(
                    m_authorityProxyModel->rowCount(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem())) - 1, 1, m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    } else {
        insert = m_authorityProxyModel->insertRows(0, 1, m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    }

    if(insert) {
        QModelIndex currentIndex;
        if(m_authorityProxyModel->sortOrder() == Qt::AscendingOrder) {
            currentIndex = m_authorityProxyModel->index(m_authorityProxyModel->rowCount(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem())) - 1, 0, m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
        } else {
           currentIndex = m_authorityProxyModel->index(0, 0, m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
        }

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

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_authorityProxyModel->removeRow(index.row(), m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
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
