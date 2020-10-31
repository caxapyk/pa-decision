#include "doctypedialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "models/colorpickeritemdelegate.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlError>

DoctypeDialog::DoctypeDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    ui->setupUi(this);

    m_model = new DoctypeModel;
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tV_itemView->hideColumn(0);
    ui->tV_itemView->resizeColumnToContents(1);
    ui->tV_itemView->setItemDelegateForColumn(2, new ColorPickerItemDelegate);

    setupShortcuts();
    restoreDialogState();

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &DoctypeDialog::contextMenu);
    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &DoctypeDialog::changeCurrent);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &DoctypeDialog::accept);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &DoctypeDialog::reject);
}

DoctypeDialog::~DoctypeDialog()
{
    //delete ui;
    delete m_model;
    delete m_proxyModel;
}

void DoctypeDialog::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &DoctypeDialog::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &DoctypeDialog::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &DoctypeDialog::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &DoctypeDialog::refresh);
}

void DoctypeDialog::changeCurrent(const QModelIndex &current, const QModelIndex &)
{
    insertShortcut->setEnabled(!current.isValid());
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);
}

void DoctypeDialog::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_itemView->indexAt(ui->tV_itemView->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_itemView->setCurrentIndex(currentIndex);

    CustomContextMenu menu(CustomContextMenu::All);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, &DoctypeDialog::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &DoctypeDialog::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &DoctypeDialog::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, &DoctypeDialog::refresh);

    menu.exec(QCursor().pos());
}

void DoctypeDialog::edit()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ui->tV_itemView->edit(index);
}

void DoctypeDialog::insert()
{
    bool insert = m_proxyModel->sourceModel()->insertRow(0);

    if(insert) {
        QModelIndex currentIndex = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, 1));

        ui->tV_itemView->resizeColumnToContents(1);

        ui->tV_itemView->setCurrentIndex(currentIndex);
        ui->tV_itemView->scrollTo(currentIndex);
        ui->tV_itemView->edit(ui->tV_itemView->currentIndex());
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void DoctypeDialog::refresh()
{
    ui->tV_itemView->selectionModel()->clearCurrentIndex();

    m_proxyModel->invalidate();
    m_model->select();
}

void DoctypeDialog::remove()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_proxyModel->removeRow(index.row());
        if (remove) {
            m_model->select();
        } else {
            QMessageBox::warning(this,
                    tr("Deleting item"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}

void DoctypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DoctypeDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("DoctypeDialog/tV_itemView").toByteArray());
}

void DoctypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DoctypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}

void DoctypeDialog::accept()
{
    saveDialogState();
    ReferenceDialog::accept();
}

void DoctypeDialog::reject()
{
    saveDialogState();
    ReferenceDialog::reject();
}

void DoctypeDialog::closeEvent(QCloseEvent *event)
{
   saveDialogState();
   ReferenceDialog::closeEvent(event);
}

