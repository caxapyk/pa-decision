#include "recorddialog.h"
#include "ui_treedialog.h"

#include "application.h"
#include "dialogs/funddetailsdialog.h"
#include "dialogs/inventorydetailsdialog.h"
#include "dialogs/recorddetailsdialog.h"
#include "dialogs/treedialog.h"
#include "dialogs/protocoldialog.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QIcon>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

RecordDialog::RecordDialog(QWidget *parent) :
     TreeDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Archive records"));
    setWindowIcon(QIcon(":/icons/icons/record-16.png"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &RecordDialog::details);

    pB_protocol = new QPushButton(tr("Protocols"));
    pB_protocol->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_protocol);

    connect(pB_protocol, &QPushButton::clicked, this, &RecordDialog::protocols);

    m_model = new RecordModel;

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(2);
    ui->tV_itemView->hideColumn(3);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setProxyModel(m_proxyModel);
}

RecordDialog::~RecordDialog()
{
    saveDialogState();

    delete pB_details;
    delete pB_protocol;

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
    if(m_authorityId.isValid()) {
        clearInfoText();

        m_model->setAuthorityId(m_authorityId.toInt());
        m_model->select();

        clearSelection();
    }

    return TreeDialog::exec();
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
    pB_protocol->setEnabled(!selected.isEmpty() && node->level == RecordModel::RecordLevel);

    if(!selected.isEmpty()) {
        QModelIndex index = ui->tV_itemView->currentIndex();
        setInfoText(index.siblingAtColumn(3).data().toString());
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
    QModelIndex index = ui->tV_itemView->currentIndex();

    switch (node->level) {
    case RecordModel::FundLevel:
    {
        QVariant id = index.siblingAtColumn(2).data(); //id

        FundDetailsDialog dialog(id);
        int res = dialog.exec();

        if(res == FundDetailsDialog::Accepted) {
            m_proxyModel->setData(index.siblingAtColumn(0), dialog.getNumber());
            setInfoText(dialog.getName());
        }
    }
        break;
    case RecordModel::InventoryLevel:
    {
        QVariant id = index.siblingAtColumn(2).data(); //id

        InventoryDetailsDialog dialog(id);
        int res = dialog.exec();

        if(res == InventoryDetailsDialog::Accepted) {
            m_proxyModel->setData(index.siblingAtColumn(0), dialog.getNumber());
            setInfoText(dialog.getName());
        }
    }
        break;
    case RecordModel::RecordLevel:
    {
        QVariant id = index.siblingAtColumn(2).data(); //id

        RecordDetailsDialog dialog(id);
        int res = dialog.exec();

        if(res == RecordDetailsDialog::Accepted) {
            m_proxyModel->setData(index.siblingAtColumn(0), dialog.getNumber());
            setInfoText(dialog.getName());
        }
    }
        break;
    }
}

void RecordDialog::protocols()
{
    QModelIndex index = ui->tV_itemView->currentIndex();
    QVariant id = index.data(Qt::UserRole); //id

    ProtocolDialog dialog(id);

    dialog.exec();
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
    TreeDialog::remove();
}
