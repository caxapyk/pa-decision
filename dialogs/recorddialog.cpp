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

    addCommentButton();
    setCommentColumn(1);

    m_model = new RecordModel;

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(2);
    ui->tV_itemView->hideColumn(3);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setDialogModel(m_proxyModel);
}

RecordDialog::~RecordDialog()
{
    saveDialogState();

    delete m_headerWidget;
    delete pB_details;

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

int RecordDialog::exec()
{
    if(m_authorityId < 0) {
        m_headerWidget = new DialogHeader;
        ui->hL_header->addWidget(m_headerWidget);

        connect(m_headerWidget, &DialogHeader::authorityChanged, this, &ReferenceDialog::loadByAuthorityId);

        loadByAuthorityId(m_headerWidget->id());
    } else {
        loadByAuthorityId(m_authorityId);
    }

    return ReferenceDialog::exec();
}

void RecordDialog::selected(const QItemSelection &selected, const QItemSelection &)
{
    QModelIndex current = !selected.isEmpty() ? selected.indexes().at(0) : QModelIndex();
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    insertShortcut->setEnabled(node == nullptr || node->level != RecordModel::RecordLevel);
    editShortcut->setEnabled(!selected.isEmpty());
    removeShortcut->setEnabled(!selected.isEmpty());
    refreshShortcut->setEnabled(true);

    pB_details->setEnabled(!selected.isEmpty());
    commentButton()->setEnabled(!selected.isEmpty());

    if(!selected.isEmpty()) {
        QModelIndex fundIndex;
        switch (node->level) {
        case RecordModel::FundLevel:
            fundIndex = ui->tV_itemView->currentIndex();
            break;
        case RecordModel::InventoryLevel:
            fundIndex = ui->tV_itemView->currentIndex().parent();
            break;
        case RecordModel::RecordLevel:
            fundIndex = ui->tV_itemView->currentIndex().parent().parent();
            break;
        }

        setInfoText(fundIndex.siblingAtColumn(3).data().toString());
    } else {
        clearInfoText();
    }
}

bool RecordDialog::choiceButtonEnabled()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()).internalPointer());

    return !isChoiceMode() || (node && node->level == RecordModel::RecordLevel);
}

int RecordDialog::choice(const QItemSelection &selected) const
{
    if(!selected.isEmpty()) {
        QModelIndex current = selected.indexes().at(0).siblingAtColumn(2);
        return m_proxyModel->mapToSource(current).data().toInt();
    }

    return -1;
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
    int res = dialog.exec();

    if(res == RecordDetailsDialog::Accepted) {
         QString n = dialog.getFundName();
         m_proxyModel->setData(index.siblingAtColumn(3), n);
         setInfoText(n);
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
