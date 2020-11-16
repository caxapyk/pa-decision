#include "_authoritydialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/authoritydetailsdialog.h"
#include "dialogs/recorddialog.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>

_AuthorityDialog::_AuthorityDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Public authorities"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    m_model = new _AuthorityModel;
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(2);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tV_itemView->expandAll();

    setDialogModel(m_proxyModel);

    connect(pB_details, &QPushButton::clicked, this, &_AuthorityDialog::details);
    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);
}

_AuthorityDialog::~_AuthorityDialog()
{
    saveDialogState();

    delete pB_details;

    delete m_model;
    delete m_proxyModel;
}

void _AuthorityDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("_AuthorityDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("_AuthorityDialog/tV_itemView").toByteArray());
}

void _AuthorityDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("_AuthorityDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}

void _AuthorityDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    //QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    QModelIndex root = m_proxyModel->mapFromSource(QModelIndex());

    refreshShortcut->setEnabled(true);
    insertShortcut->setEnabled(!current.isValid() || current == root);
    editShortcut->setEnabled(current.isValid() && current !=root);
    removeShortcut->setEnabled(current.isValid() && current != root);

    pB_details->setEnabled(current.isValid() && current != root);
    setComment(current.siblingAtColumn(1).data().toString());
}

void _AuthorityDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    AuthorityDetailsDialog dialog(index.data(Qt::UserRole));
    int res = dialog.exec();

    if(res == _AuthorityDialog::Accepted) {
        setComment(dialog.comment());
    }
}
