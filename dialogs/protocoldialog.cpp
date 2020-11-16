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
    ui->label_infoIcon->setVisible(false);

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &ProtocolDialog::details);

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
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setDialogModel(m_proxyModel);
    addCommentButton();
    setCommentColumn(5);
    loadByAuthorityId(m_headerWidget->id());
}

ProtocolDialog::~ProtocolDialog()
{
    saveDialogState();

    delete pB_details;
    delete pB_comment;

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

void ProtocolDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    insertShortcut->setEnabled(true);
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);

    pB_details->setEnabled(current.isValid());
    commentButton()->setEnabled(current.isValid());

    setComment(current.siblingAtColumn(5).data().toString());
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

int ProtocolDialog::choice(const QModelIndex &current) const
{
    return m_proxyModel->mapToSource(current).data(Qt::UserRole).toInt();
}

void ProtocolDialog::insert()
{
    ProtocolDetailsDialog dialog;

    dialog.setModel(m_model);
    dialog.exec();
}
