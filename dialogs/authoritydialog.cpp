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

    pB_comment = new QPushButton(tr("Comment"));
    pB_comment->setIcon(QIcon(":/icons/icons/comment-16.png"));
    pB_comment->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_comment);

    connect(pB_comment, &QPushButton::clicked, this, &AuthorityDialog::editComment);

    m_model = new AuthorityModel;
    m_model->select();

    m_proxyModel = new AuthorityProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(2);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tV_itemView->setCurrentIndex(m_proxyModel->mapFromSource(m_model->rootItem()));
    ui->tV_itemView->expandAll();

    setDialogModel(m_proxyModel);

    connect(pB_details, &QPushButton::clicked, this, &AuthorityDialog::details);
    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);
}

AuthorityDialog::~AuthorityDialog()
{
    saveDialogState();

    delete pB_details;
    delete pB_comment;

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

void AuthorityDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    QModelIndex root = m_proxyModel->mapFromSource(m_model->rootItem());

    refreshShortcut->setEnabled(true);
    insertShortcut->setEnabled(!current.isValid() || current == root);
    editShortcut->setEnabled(current.isValid() && current !=root);
    removeShortcut->setEnabled(current.isValid() && current != root);

    pB_details->setEnabled(current.isValid() && current != root);
    pB_comment->setEnabled(current.isValid() && current != root);

    setComment(current.siblingAtColumn(1).data().toString());
}

void AuthorityDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    AuthorityDetailsDialog dialog(index.data(Qt::UserRole));
    int res = dialog.exec();

    if(res == AuthorityDialog::Accepted) {
        setComment(dialog.comment());
    }
}

void AuthorityDialog::editComment()
{
    QModelIndex index = ui->tV_itemView->currentIndex().siblingAtColumn(1);
    QString title = tr("Comment");

    QVariant value  = inputDialog(title, tr("Enter comment"), index.data());

    if (value.isValid() && value != index.data()) {
        bool set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(index), value);
        if(set) {
            setComment(value.toString());
        } else {
            QMessageBox::warning(this,
                                 title,
                                 tr("Could not set data.") + (value.toString().length() >= 255 ? tr(" Too long.") : ""),
                                 QMessageBox::Ok);
        }
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
