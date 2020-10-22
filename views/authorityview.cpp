#include "application.h"

#include "authorityview.h"
#include "ui_authorityview.h"

#include <QDebug>
#include <QMessageBox>

AuthorityView::AuthorityView(QWidget *parent) :
    View(parent),
    ui(new Ui::AuthorityView)
{
    ui->setupUi(this);

    initialize();
    setupActions();
}

AuthorityView::~AuthorityView()
{
    delete ui;
    delete m_authorityModel;
}

void AuthorityView::initialize()
{
    m_authorityModel = new AuthorityTreeModel();
    m_authorityModel->select();
    ui->tV_tree->setModel(m_authorityModel);
    ui->tV_tree->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_tree, &QMenu::customContextMenuRequested, this, &AuthorityView::contextMenu);
}

void AuthorityView::contextMenu()
{
    QModelIndex currentIndex = currentIndex();

    QMenu menu;

    menu.addAction(&openAction);
    openAction.setEnabled(currentIndex.isValid());
    connect(&openAction, &QAction::triggered, this, [=] {
            open(currentIndex);
        });

    menu.addAction(&refreshAction);
    connect(&refreshAction, &QAction::triggered, this, &AuthorityView::refresh);

    menu.addSeparator();

    menu.addAction(&appendAction);
    appendAction.setEnabled(currentIndex.isValid() && currentIndex == m_authorityModel->rootItem());
    connect(&appendAction, &QAction::triggered, this, &AuthorityView::insert);

    menu.addAction(&editAction);
    editAction.setEnabled(currentIndex.isValid() && currentIndex != m_authorityModel->rootItem());
    connect(&editAction, &QAction::triggered, this, [=] {
        edit(currentIndex);
    });

    menu.addAction(&removeAction);
    removeAction.setEnabled(currentIndex.isValid() && currentIndex != m_authorityModel->rootItem());
    connect(&removeAction, &QAction::triggered, this, [=] {
        remove(currentIndex);
    });
    menu.addSeparator();


    menu.addAction(&infoAction);
    infoAction.setEnabled(currentIndex.isValid() && currentIndex != m_authorityModel->rootItem());

    menu.exec(QCursor().pos());
}

QModelIndex AuthorityView::currentIndex()
{
    return ui->tV_tree->indexAt(ui->tV_tree->viewport()->mapFromGlobal(QCursor().pos()));
}

void AuthorityView::setupActions()
{
    openAction.setIcon(QIcon(":/icons/icons/open-16.png"));
    openAction.setText(tr("Open"));
    openAction.setShortcut(QKeySequence::Open);
    //connect(&openAction, &QAction::triggered, this, [=] {
    //        open(currentIndex);
    //    });

    refreshAction.setIcon(QIcon(":/icons/icons/refresh-16.png"));
    refreshAction.setText(tr("Refresh"));
    refreshAction.setShortcut(QKeySequence::Refresh);
    //connect(&refreshAction, &QAction::triggered, this, &AuthorityView::refresh);

    appendAction.setIcon(QIcon(":/icons/icons/add-16.png"));
    appendAction.setText(tr("New"));
    appendAction.setShortcut(QKeySequence::New);
    //connect(&appendAction, &QAction::triggered, this, &AuthorityView::insert);

    editAction.setIcon(QIcon(":/icons/icons/edit-16.png"));
    editAction.setText(tr("Edit"));
    editAction.setShortcut(QKeySequence(Qt::Key_F2));
    //connect(&editAction, &QAction::triggered, this, [=] {
    //    edit(currentIndex);
    //});

    removeAction.setIcon(QIcon(":/icons/icons/remove-16.png"));
    removeAction.setText(tr("Remove"));
    removeAction.setShortcut(QKeySequence::Delete);
    //connect(&removeAction, &QAction::triggered, this, [=] {
    //    remove(currentIndex);
    //});

     infoAction.setText(tr("Info"));
}

void AuthorityView::open(const QModelIndex &index)
{
    if(index == m_authorityModel->rootItem()) {
        ui->tV_tree->expand(m_authorityModel->rootItem());
    } else {
        // load data
    }
}

void AuthorityView::refresh()
{
    m_authorityModel->select();
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

void AuthorityView::edit(const QModelIndex &index)
{
    ui->tV_tree->edit(index);
}

void AuthorityView::remove(const QModelIndex &index)
{
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

}
