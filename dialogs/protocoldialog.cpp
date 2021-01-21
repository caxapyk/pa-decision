#include "protocoldialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/protocoldetailsdialog.h"

#include <QDebug>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

ProtocolDialog::ProtocolDialog(QWidget *parent, const QVariant &recordId) :
    ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Protocols"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &ProtocolDialog::details);

    m_model = new ProtocolModel;
    if(recordId.isValid()) {
        m_model->setRecordId(recordId);
    }
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(0);
    ui->tV_itemView->hideColumn(6);

    setDialogModel(m_proxyModel);
}

ProtocolDialog::~ProtocolDialog()
{
    saveDialogState();

    delete pB_details;

    delete m_model;
    delete m_proxyModel;
}

void ProtocolDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("ProtocolDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("ProtocolDialog/tV_itemView").toByteArray());
}

void ProtocolDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("ProtocolDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}

int ProtocolDialog::exec()
{
    if(m_authorityId.isValid()) {
        clearInfoText();

        m_model->setAuthorityId(m_authorityId.toInt());
        m_model->select();

        clearSelection();
    }

    return ReferenceDialog::exec();
}

void ProtocolDialog::selected(const QItemSelection &selected, const QItemSelection &deselected)
{
    pB_details->setEnabled(!selected.isEmpty());

    ReferenceDialog::selected(selected, deselected);
}

void ProtocolDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ProtocolDetailsDialog dialog;

    dialog.setModel(m_model);
    dialog.setCurrentIndex(m_proxyModel->mapToSource(index));
    dialog.exec();
}

bool ProtocolDialog::choiceButtonEnabled()
{
    return !isChoiceMode() || !ui->tV_itemView->selectionModel()->selection().isEmpty();
}

int ProtocolDialog::choice(const QItemSelection &selected) const
{
    if(!selected.isEmpty()) {
        QModelIndex current = selected.indexes().at(0).siblingAtColumn(0);
        return m_proxyModel->mapToSource(current).data().toInt();
    }

    return -1;
}

void ProtocolDialog::insert()
{
    ProtocolDetailsDialog dialog;

    dialog.setModel(m_model);
    int res = dialog.exec();

    if(res == ProtocolDetailsDialog::Accepted) {
        QModelIndex topLeft = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, 0));
        QModelIndex bottomRight = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, m_proxyModel->columnCount() - 1));

        QItemSelection selection(topLeft, bottomRight);
        ui->tV_itemView->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
        ui->tV_itemView->scrollTo(topLeft);
    }
}
