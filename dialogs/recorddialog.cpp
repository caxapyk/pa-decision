#include "recorddialog.h"

#include "application.h"
#include "dialogs/funddetailsdialog.h"
#include "dialogs/inventorydetailsdialog.h"
#include "dialogs/recorddetailsdialog.h"
#include "dialogs/treedialog.h"
#include "dialogs/protocoldialog.h"
#include "utils/basecontextmenu.h"

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

    m_tree->setModel(m_proxyModel);
    m_tree->hideColumn(1);
    m_tree->hideColumn(2);
    m_tree->hideColumn(3);
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);

    setProxyModel(m_proxyModel);

    clearSelection();
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
    m_tree->header()->restoreState(settings->value("RecordDialog/tV_itemView").toByteArray());
}

void RecordDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("RecordDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", m_tree->header()->saveState());
    settings->endGroup();
}

int RecordDialog::exec()
{
    if(isChoiceMode())
        m_tree->expandAll();

    return TreeDialog::exec();
}

void RecordDialog::contextMenu(BaseContextMenu &menu)
{
    QModelIndex currentIndex = m_tree->indexAt(m_tree->viewport()->mapFromGlobal(QCursor().pos()));
    m_tree->setCurrentIndex(currentIndex);

    if(isChoiceMode() && !currentIndex.isValid() && m_choiceLevel != RecordModel::FundLevel) {
        menu.action(BaseContextMenu::Insert)->setDisabled(true);
    }

    menu.exec(QCursor().pos());
}

void RecordDialog::selected(const QItemSelection &selected, const QItemSelection &)
{
    QModelIndex current = !selected.isEmpty() ? selected.indexes().at(0) : QModelIndex();
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    int level;
    if(node == nullptr) {
        level = RecordModel::FundLevel;
    } else {
        level = node->level;
    }

    m_tree->setInsertEnabled((level != RecordModel::RecordLevel) && (!isChoiceMode() || (isChoiceMode() && level == m_choiceLevel - 1)));
    m_tree->setEditEnabled(!selected.isEmpty() && (!isChoiceMode() || (isChoiceMode() && level == m_choiceLevel)));
    m_tree->setRemoveEnabled(!selected.isEmpty() && (!isChoiceMode() || (isChoiceMode() && level == m_choiceLevel)));
    m_tree->setRefreshEnabled(true);

    pB_details->setEnabled(!selected.isEmpty());
    pB_protocol->setEnabled(!isChoiceMode() && !selected.isEmpty() && level == RecordModel::RecordLevel);

    if(!selected.isEmpty()) {
        QModelIndex index = m_tree->currentIndex();
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
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(m_tree->currentIndex()).internalPointer());

    return !isChoiceMode() || (node && node->level == m_choiceLevel);
}

QVariant RecordDialog::choice(const QItemSelection &selected) const
{
    if(!selected.isEmpty()) {
        QModelIndex current = selected.indexes().at(0).siblingAtColumn(2);
        return m_proxyModel->mapToSource(current).data();
    }

    return QVariant();
}

void RecordDialog::details()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(m_tree->currentIndex()).internalPointer());
    QModelIndex index = m_tree->currentIndex();

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
    QModelIndex index = m_tree->currentIndex();
    QVariant id = index.data(Qt::UserRole); //id

    ProtocolDialog dialog(id);

    dialog.exec();
}

void RecordDialog::insert()
{
    QModelIndex proxyParent = m_tree->currentIndex();
    QModelIndex sourceParent = m_proxyModel->mapToSource(proxyParent);

    if(!m_tree->isExpanded(proxyParent)) {
        m_tree->expand(proxyParent);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceParent);

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceParent);

    if(insert) {
        QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceParent));

        m_tree->setCurrentIndex(currentIndex);
        m_tree->scrollTo(currentIndex);
        m_tree->edit(m_tree->currentIndex());
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
