#include "authoritydialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"
#include "dialogs/recorddialog.h"

#include <QMenu>
#include <QMessageBox>

AuthorityDialog::AuthorityDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    setWindowTitle(tr("Public authorities"));
    ui->label_infoIcon->setVisible(false);

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);
    ui->vL_buttonGroup->addWidget(pB_details);

    m_model = new AuthorityModel;
    m_model->select();

    m_proxyModel = new AuthorityProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tV_itemView->setCurrentIndex(m_proxyModel->mapFromSource(m_model->rootItem()));
    ui->tV_itemView->expandAll();

    connect(pB_details, &QPushButton::clicked, this, &AuthorityDialog::details);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);
    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &AuthorityDialog::changeCurrent);
}

AuthorityDialog::~AuthorityDialog()
{
    delete m_model;
    delete m_proxyModel;
    delete pB_details;
}

void AuthorityDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("AuthorityDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("AuthorityDialog/tV_itemView").toByteArray());
}

void AuthorityDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("AuthorityDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}

void AuthorityDialog::changeCurrent(const QModelIndex &current, const QModelIndex &)
{
    QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    refreshShortcut->setEnabled(true);
    insertShortcut->setEnabled(current.isValid() && current == root);
    editShortcut->setEnabled(current.isValid() && current !=root);
    removeShortcut->setEnabled(current.isValid() && current != root);

    pB_details->setEnabled(current.isValid() && current != root);
}

void AuthorityDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    AuthorityDetailsDialog dialog(index.data(Qt::UserRole));
    dialog.exec();
}

void AuthorityDialog::refresh()
{
    m_proxyModel->invalidate();
    m_model->select();

    QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    ui->tV_itemView->expandAll();
    ui->tV_itemView->setCurrentIndex(root);
}

void AuthorityDialog::insert()
{
    QModelIndex sourceRoot = m_model->rootItem();
    QModelIndex proxyRoot = m_proxyModel->mapFromSource(sourceRoot);

    if(!ui->tV_itemView->isExpanded(proxyRoot)) {
        ui->tV_itemView->expand(proxyRoot);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceRoot) - 1;

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceRoot);

    if(insert) {
        v += 1;
        QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceRoot));

        ui->tV_itemView->setCurrentIndex(currentIndex);
        ui->tV_itemView->scrollTo(currentIndex);
        ui->tV_itemView->edit(currentIndex);
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void AuthorityDialog::edit()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ui->tV_itemView->edit(index);
}

void AuthorityDialog::remove()
{
    QModelIndex index = ui->tV_itemView->currentIndex();
    QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_proxyModel->removeRow(index.row(), root);
        if (!remove) {
            QMessageBox::warning(this,
                    tr("Deleting item"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}
