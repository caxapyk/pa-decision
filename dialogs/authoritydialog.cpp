#include "authoritydialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"
#include "dialogs/recorddialog.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>

AuthorityDialog::AuthorityDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Public authorities"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &AuthorityDialog::details);

    addCommentButton();
    setCommentColumn(1);

    m_model = new AuthorityModel;
    m_model->select();

    m_proxyModel = new AuthorityProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(2);

    ui->tV_itemView->setCurrentIndex(m_proxyModel->mapFromSource(m_model->rootItem()));
    ui->tV_itemView->expandAll();

    setDialogModel(m_proxyModel);
}

AuthorityDialog::~AuthorityDialog()
{
    saveDialogState();

    delete pB_details;

    delete m_model;
    delete m_proxyModel;
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

void AuthorityDialog::selected(const QItemSelection &selected, const QItemSelection &)
{
    QModelIndex current = !selected.isEmpty() ? selected.indexes().at(0) : QModelIndex();
    QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    refreshShortcut->setEnabled(true);
    insertShortcut->setEnabled(selected.isEmpty() || current == root);
    editShortcut->setEnabled(!selected.isEmpty() && current !=root);
    removeShortcut->setEnabled(!selected.isEmpty() && current != root);

    pB_details->setEnabled(!selected.isEmpty() && current != root);
    commentButton()->setEnabled(!selected.isEmpty() && current != root);
}

void AuthorityDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    AuthorityDetailsDialog dialog(index.data(Qt::UserRole));
    int res = dialog.exec();

    if(res == AuthorityDetailsDialog::Accepted) {
        m_proxyModel->setData(index, dialog.getName());
    }
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

void AuthorityDialog::refresh()
{
    ReferenceDialog::refresh();
    ui->tV_itemView->expandAll();
}
