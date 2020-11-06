#include "recorddialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "dialogs/referencedialog.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

RecordDialog::RecordDialog(QWidget *parent) :
     ReferenceDialog(parent)
{
    setWindowTitle(tr("Archive records"));

    pB_fundTitle = new QPushButton(tr("Title"));
    pB_fundTitle->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_fundTitle);

    m_headerWidget = new DialogHeader;
    ui->hL_header->addWidget(m_headerWidget);

    connect(m_headerWidget, &DialogHeader::authorityChanged, this, &RecordDialog::loadByAuthorityId);
    m_model = new RecordModel;

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    setDialogModel(m_proxyModel);

    connect(pB_fundTitle, &QPushButton::clicked, this, &RecordDialog::editFundName);
    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);

    loadByAuthorityId(m_headerWidget->id());
}

RecordDialog::~RecordDialog()
{
    delete m_headerWidget;
    delete pB_fundTitle;
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

void RecordDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    insertShortcut->setEnabled(node == nullptr || node->level != RecordModel::RecordLevel);
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);

    pB_fundTitle->setEnabled(current.isValid() && (node != nullptr && node->level == RecordModel::FundLevel));
    commentButton()->setEnabled(current.isValid());

    m_headerWidget->setFundName((current.isValid() && node->level == RecordModel::FundLevel) ? current.data(ReferenceModel::InfoRole).toString() : QString());
}

bool RecordDialog::choiceButtonEnabled()
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()).internalPointer());
    bool e = !isChoiceMode() || node->level == RecordModel::RecordLevel;

    return e;
}

int RecordDialog::choice(const QModelIndex &current) const
{
    return m_proxyModel->mapToSource(current).data(ReferenceModel::IDRole).toInt();
}

void RecordDialog::loadByAuthorityId(int id)
{
    ui->tV_itemView->selectionModel()->clearCurrentIndex();

    m_model->setAuthorityId(id);
    m_model->select();
}

void RecordDialog::editFundName()
{
    QModelIndex index = ui->tV_itemView->currentIndex();
    QString title = tr("Fund name");

    QString value = inputDialog(title, tr("Enter fund name"), index.data(ReferenceModel::InfoRole).toString());

    if (!value.isEmpty()) {
        bool set;
        set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()), value, ReferenceModel::InfoRole);

        if(set) {
            m_headerWidget->setFundName(value);
        } else {
            QMessageBox::warning(this,
                    title,
                    tr("Could not set data.") + (value.length() >= 255 ? tr(" Too long.") : ""),
                    QMessageBox::Ok);
        }
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
