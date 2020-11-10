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

    connect(pB_details, &QPushButton::clicked, this, &ProtocolDialog::details);

    ui->vL_buttonGroup->addWidget(pB_details);

    m_headerWidget = new DialogHeader;
    ui->hL_header->addWidget(m_headerWidget);

    connect(m_headerWidget, &DialogHeader::authorityChanged, this, &ProtocolDialog::loadByAuthorityId);

    m_model = new ProtocolModel;

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(3);
    ui->tV_itemView->hideColumn(4);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setDialogModel(m_proxyModel);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);

    loadByAuthorityId(m_headerWidget->id());
}

ProtocolDialog::~ProtocolDialog()
{
    saveDialogState();

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

    setComment(current.siblingAtColumn(3).data().toString());
}

void ProtocolDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ProtocolDetailsDialog dialog(m_model, index.row());
    int res = dialog.exec();

    if(res == ProtocolDetailsDialog::Accepted && dialog.property().contains("comment")) {
        setComment(dialog.property().value("comment").toString());
    }
}

bool ProtocolDialog::choiceButtonEnabled()
{
    return !isChoiceMode() || ui->tV_itemView->currentIndex().isValid();
}

int ProtocolDialog::choice(const QModelIndex &current) const
{
    return m_proxyModel->mapToSource(current).data(Qt::UserRole).toInt();
}

void ProtocolDialog::loadByAuthorityId(int id)
{
    clearInfoText();

    m_model->setAuthorityId(id);
    m_model->select();

    selected(QModelIndex(), QModelIndex());
}

void ProtocolDialog::edit()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ui->tV_itemView->edit(index);
}

void ProtocolDialog::insert()
{
    QModelIndex proxyParent = ui->tV_itemView->currentIndex();
    QModelIndex sourceParent = m_proxyModel->mapToSource(proxyParent);

    if(!ui->tV_itemView->isExpanded(proxyParent)) {
        ui->tV_itemView->expand(proxyParent);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceParent);

    ProtocolDetailsDialog dialog(m_model, v);
    int res = dialog.exec();

    if(res == ProtocolDetailsDialog::Accepted && dialog.property().contains("comment")) {
        setComment(dialog.property().value("comment").toString());
    }

    QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceParent));

    ui->tV_itemView->setCurrentIndex(currentIndex);
    ui->tV_itemView->scrollTo(currentIndex);
}

void ProtocolDialog::editComment()
{
    QModelIndex index = ui->tV_itemView->currentIndex().siblingAtColumn(3);
    QString title = tr("Comment");

    QVariant value = inputDialog(title, tr("Enter comment"), index.data());

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
