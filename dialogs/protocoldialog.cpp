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

    pB_comment = new QPushButton(tr("Comment"));
    pB_comment->setIcon(QIcon(":/icons/icons/comment-16.png"));
    pB_comment->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_comment);

    connect(pB_comment, &QPushButton::clicked, this, &ProtocolDialog::editComment);

    m_headerWidget = new DialogHeader;
    ui->hL_header->addWidget(m_headerWidget);

    connect(m_headerWidget, &DialogHeader::authorityChanged, this, &ProtocolDialog::loadByAuthorityId);

    m_model = new ProtocolModel;

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(0);
    ui->tV_itemView->hideColumn(1);
     ui->tV_itemView->hideColumn(5);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setDialogModel(m_proxyModel);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);

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
    pB_comment->setEnabled(current.isValid());

    setComment(current.siblingAtColumn(5).data().toString());
}

void ProtocolDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ProtocolDetailsDialog dialog;
    dialog.setModel(m_model);
    dialog.setCurrentIndex(m_proxyModel->mapToSource(index));

    int res = dialog.exec();

    if(res == ProtocolDialog::Accepted) {
        //refresh();
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
    ProtocolDetailsDialog dialog;
    dialog.setModel(m_model);

    int res = dialog.exec();

    if(res == ProtocolDialog::Accepted) {
        //refresh();
    }
}

void ProtocolDialog::editComment()
{
    QModelIndex index = ui->tV_itemView->currentIndex().siblingAtColumn(5);
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
