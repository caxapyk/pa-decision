#include "recorddialog.h"

#include "application.h"
#include "dialogs/funddetailsdialog.h"
#include "dialogs/inventorydetailsdialog.h"
#include "dialogs/recorddetailsdialog.h"
#include "dialogs/treedialog.h"
#include "dialogs/protocoldialog.h"
#include "widgets/basecontextmenu.h"

#include <QDebug>
#include <QIcon>
#include <QItemSelection>
#include <QHeaderView>
#include <QMessageBox>

RecordDialog::RecordDialog(const QVariant &authorityId, const QVariant &fundId, const QVariant &inventoryId, QWidget *parent) :
     TreeDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Archive records"));
    setWindowIcon(QIcon(":/icons/icons/record-16.png"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    buttonLayout()->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &RecordDialog::details);

    pB_protocol = new QPushButton(tr("Protocols"));
    pB_protocol->setDisabled(true);

    buttonLayout()->addWidget(pB_protocol);

    connect(pB_protocol, &QPushButton::clicked, this, &RecordDialog::protocols);

    m_model = new RecordModel;
    m_model->setAuthorityId(authorityId);
    m_model->setFundId(fundId);
    m_model->setInventoryId(inventoryId);
    m_model->select();

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    treeView()->setModel(m_proxyModel);
    treeView()->hideColumn(1);
    treeView()->hideColumn(2);
    treeView()->hideColumn(3);
    treeView()->setContextMenuPolicy(Qt::CustomContextMenu);

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
    treeView()->header()->restoreState(settings->value("RecordDialog/tV_itemView").toByteArray());
}

void RecordDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("RecordDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", treeView()->header()->saveState());
    settings->endGroup();
}

int RecordDialog::exec()
{
    if(isChoiceMode())
        treeView()->expandAll();

    return TreeDialog::exec();
}

void RecordDialog::onCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    int level;
    if(node == nullptr) {
        level = RecordModel::FundLevel - 1;
    } else {
        level = node->level;
    }

    treeView()->setInsertEnabled((level != RecordModel::RecordLevel) && (!isChoiceMode() || (isChoiceMode() && ((level < 0 && choiceLevel() == RecordModel::FundLevel) || (level == choiceLevel() - 1)))));
    treeView()->setEditEnabled(current.isValid() && (!isChoiceMode() || (isChoiceMode() && level == choiceLevel())));
    treeView()->setRemoveEnabled(current.isValid() && (!isChoiceMode() || (isChoiceMode() && level == choiceLevel())));
    treeView()->setRefreshEnabled(true);

    pB_details->setEnabled(current.isValid());
    pB_protocol->setEnabled(!isChoiceMode() && current.isValid() && level == RecordModel::RecordLevel);

    if(current.isValid()) {
        QModelIndex index = treeView()->currentIndex();
        setInfoText(index.siblingAtColumn(3).data().toString());
    } else {
        clearInfoText();
    }
}

void RecordDialog::setChoiceLevel(RecordModel::Levels level)
{
    switch (level) {
        case RecordModel::FundLevel: m_model->setMaxDepth(1); break;
        case RecordModel::InventoryLevel: m_model->setMaxDepth(2); break;
        case RecordModel::RecordLevel: m_model->setMaxDepth(3); break;
    }

    m_choiceLevel = level;
}

bool RecordDialog::choiceButtonEnabled()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(treeView()->currentIndex()).internalPointer());

    return !isChoiceMode() || (node && node->level == choiceLevel());
}

QVariant RecordDialog::choice(const QModelIndex &current) const
{
    if(current.isValid()) {
        return m_proxyModel->mapToSource(current.siblingAtColumn(2)).data();
    }

    return QVariant();
}

void RecordDialog::details()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(treeView()->currentIndex()).internalPointer());
    QModelIndex index = treeView()->currentIndex();

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
    QModelIndex index = treeView()->currentIndex();
    QVariant id = index.data(Qt::UserRole); //id

    ProtocolDialog dialog(id);

    dialog.exec();
}

void RecordDialog::insert()
{
    QModelIndex proxyParent = treeView()->currentIndex();
    QModelIndex sourceParent = m_proxyModel->mapToSource(proxyParent);

    if(!treeView()->isExpanded(proxyParent)) {
        treeView()->expand(proxyParent);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceParent);

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceParent);

    if(insert) {
        QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceParent));

        treeView()->setCurrentIndex(currentIndex);
        treeView()->scrollTo(currentIndex);
        treeView()->edit(treeView()->currentIndex());
    } else {
        QMessageBox::warning(this,
                tr("Archive records"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void RecordDialog::remove()
{
    clearInfoText();
    TreeDialog::remove();
}
