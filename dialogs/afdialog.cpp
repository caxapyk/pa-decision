#include "afdialog.h"

#include "application.h"
#include "dialogs/choicedialog.h"
#include "dialogs/funddetailsdialog.h"
#include "dialogs/inventorydetailsdialog.h"
#include "dialogs/recorddetailsdialog.h"
#include "dialogs/protocoldialog.h"

#include <QDebug>
#include <QIcon>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QMessageBox>

AFDialog::AFDialog(QWidget *parent) : ChoiceDialog(parent)
{
    m_view = new AFView;
    setTreeView(m_view);

    restoreDialogState();

    setWindowTitle(tr("Archive records"));
    setWindowIcon(QIcon(":/icons/icons/record-16.png"));

    pB_fund = new QPushButton(tr("Create fund"));
    pB_fund->setEnabled(true);

    buttonLayout()->addWidget(pB_fund);

    connect(pB_fund, &QPushButton::clicked, this, [=] {
        m_view->setCurrentIndex(QModelIndex());
        m_view->insertRow();
    });

    pB_protocol = new QPushButton(tr("Protocols"));
    pB_protocol->setDisabled(true);

    buttonLayout()->addWidget(pB_protocol);

    connect(pB_protocol, &QPushButton::clicked, this, [=] {
        m_view->protocols();
    });

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    buttonLayout()->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, [=] {
        m_view->details();
    });
}

AFDialog::~AFDialog()
{
    saveDialogState();

    delete pB_details;
    delete pB_protocol;
    delete pB_fund;

    delete m_view;
}

void AFDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("AFDialog/geometry").toByteArray());
}

void AFDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("AFDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}

int AFDialog::exec()
{
    m_view->refresh();

    if(isChoiceMode()) {
        m_view->expandAll();
        pB_fund->setEnabled(choiceLevel() == AFTreeModel::FundLevel);
    }

    return ChoiceDialog::exec();
}

void AFDialog::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    ChoiceDialog::selectionChanged(selected, deselected);

    if(selected.isEmpty()) {
        clearInfoText();
        m_view->setInsertEnabled(false);
        m_view->setEditEnabled(false);
        m_view->setRemoveEnabled(false);

    } else {
        const QModelIndex current = selected.indexes().last();
        setInfoText(current.siblingAtColumn(3).data().toString());

        AFTreeModel::RecordNode *node = static_cast<AFTreeModel::RecordNode*>(m_view->proxyModel()->mapToSource(current).internalPointer());
        int level = (node == nullptr) ? AFTreeModel::FundLevel - 1 : node->level;

        if(isChoiceMode()) {
            m_view->setInsertEnabled(level == choiceLevel() - 1);
            m_view->setEditEnabled(level == choiceLevel());
            m_view->setRemoveEnabled(level == choiceLevel());
        } else {
             pB_protocol->setEnabled(current.isValid() && level == AFTreeModel::RecordLevel);
        }

        pB_details->setEnabled(current.isValid());

    }
}

void AFDialog::setChoiceLevel(AFTreeModel::Levels level)
{
    switch (level) {
        case AFTreeModel::FundLevel: m_view->model()->setMaxDepth(1); break;
        case AFTreeModel::InventoryLevel: m_view->model()->setMaxDepth(2); break;
        case AFTreeModel::RecordLevel: m_view->model()->setMaxDepth(3); break;
    }

    m_choiceLevel = level;
}

bool AFDialog::choiceButtonEnabled()
{
    AFTreeModel::RecordNode *node = static_cast<AFTreeModel::RecordNode*>(m_view->proxyModel()->mapToSource(m_view->currentIndex()).internalPointer());

    return !isChoiceMode() || (node && node->level == choiceLevel());
}

QVariant AFDialog::choice(const QModelIndex &current) const
{
   if(current.isValid()) {
        return m_view->proxyModel()->mapToSource(current.siblingAtColumn(2)).data();
    }

    return QVariant();
}
