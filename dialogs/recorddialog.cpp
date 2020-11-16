#include "recorddialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/recorddetailsdialog.h"
#include "dialogs/referencedialog.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QIcon>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

RecordDialog::RecordDialog(QWidget *parent) :
     ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Archive records"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &RecordDialog::details);

    pB_comment = new QPushButton(tr("Comment"));
    pB_comment->setIcon(QIcon(":/icons/icons/comment-16.png"));
    pB_comment->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_comment);

    connect(pB_comment, &QPushButton::clicked, this, &RecordDialog::editComment);

    m_headerWidget = new DialogHeader;
    ui->hL_header->addWidget(m_headerWidget);

    connect(m_headerWidget, &DialogHeader::authorityChanged, this, &RecordDialog::loadByAuthorityId);

    m_model = new RecordModel;

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(2);
    ui->tV_itemView->hideColumn(3);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setDialogModel(m_proxyModel);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);

    loadByAuthorityId(m_headerWidget->id());
}

RecordDialog::~RecordDialog()
{
    saveDialogState();

    delete m_headerWidget;
    delete pB_details;
    delete pB_comment;

    delete m_model;
    delete m_proxyModel;
}

void RecordDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("RecordDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("RecordDialog/tV_itemView").toByteArray());
}

void RecordDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("RecordDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}

void RecordDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    insertShortcut->setEnabled(node == nullptr || node->level != RecordModel::RecordLevel);
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);

    pB_details->setEnabled(current.isValid());
    pB_comment->setEnabled(current.isValid());

    if(current.isValid() && node->level == RecordModel::FundLevel)
        setInfoText(current.siblingAtColumn(3).data().toString());

    setComment(current.siblingAtColumn(1).data().toString());
}

bool RecordDialog::choiceButtonEnabled()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()).internalPointer());
    bool e = !isChoiceMode() || node->level == RecordModel::RecordLevel;

    return e;
}

int RecordDialog::choice(const QModelIndex &current) const
{
    return m_proxyModel->mapToSource(current).data(Qt::UserRole).toInt();
}

void RecordDialog::loadByAuthorityId(int id)
{
    clearInfoText();

    m_model->setAuthorityId(id);
    m_model->select();

    selected(QModelIndex(), QModelIndex());
}

void RecordDialog::details()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()).internalPointer());
    QModelIndex index;

    switch (node->level) {
    case RecordModel::FundLevel:
        index = ui->tV_itemView->currentIndex();
        break;
    case RecordModel::InventoryLevel:
        index = ui->tV_itemView->currentIndex().parent();
        break;
    case RecordModel::RecordLevel:
        index = ui->tV_itemView->currentIndex().parent().parent();
        break;
    }

    RecordDetailsDialog dialog(index.data(Qt::UserRole));
    dialog.exec();
}

void RecordDialog::editComment()
{
    QModelIndex index = ui->tV_itemView->currentIndex().siblingAtColumn(1);
    QString title = tr("Comment");

    QVariant value = inputDialog(title, tr("Enter comment"), index.data());

    if (value.isValid() && value != index.data()) {
        bool set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(index), value);
        if(set) {
            setComment(value.toString());
        } else {
            QMessageBox::warning(this,
                                 title,
                                 tr("Could not set data."),
                                 QMessageBox::Ok);
        }
    }
}

void RecordDialog::insert()
{
    QModelIndex proxyParent = ui->tV_itemView->currentIndex();
    QModelIndex sourceParent = m_proxyModel->mapToSource(proxyParent);

    if(!ui->tV_itemView->isExpanded(proxyParent)) {
        ui->tV_itemView->expand(proxyParent);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceParent);

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceParent);

    if(insert) {
        QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceParent));

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

void RecordDialog::remove()
{
    clearInfoText();
    ReferenceDialog::remove();
}
