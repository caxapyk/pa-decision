#include "protocoldialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/protocoldetailsdialog.h"

#include <QDebug>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

ProtocolDialog::ProtocolDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Protocols"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &ProtocolDialog::details);

    addCommentButton();
    setCommentColumn(5);

    m_headerWidget = new DialogHeader;
    ui->hL_header->addWidget(m_headerWidget);

    connect(m_headerWidget, &DialogHeader::authorityChanged, this, &ProtocolDialog::loadByAuthorityId);

    m_model = new StandardReferenceModel;
    m_model->setTable("pad_protocol");

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(0);
    ui->tV_itemView->hideColumn(1);
    ui->tV_itemView->hideColumn(5);

    m_model->setHeaderData(2, Qt::Horizontal, tr("Number"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("Date"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("Title"));

    setDialogModel(m_proxyModel);
    loadByAuthorityId(m_headerWidget->id());
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

void ProtocolDialog::selected(const QItemSelection &selected, const QItemSelection &deselected)
{
    pB_details->setEnabled(!selected.isEmpty());
    commentButton()->setEnabled(!selected.isEmpty());

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
    return !isChoiceMode() || ui->tV_itemView->currentIndex().isValid();
}

int ProtocolDialog::choice(const QItemSelection &selected) const
{
    return 0;
    //return m_proxyModel->mapToSource(current).data(Qt::UserRole).toInt();
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
