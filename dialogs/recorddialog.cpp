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
    ui->label_infoIcon->setVisible(false);

    pB_comment = new QPushButton(tr("Comment"));
    pB_comment->setDisabled(true);
    pB_fundTitle = new QPushButton(tr("Title"));
    pB_fundTitle->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_fundTitle);
    ui->vL_buttonGroup->addWidget(pB_comment);

    m_model = new RecordModel;
    m_model->select();

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(pB_fundTitle, &QPushButton::clicked, this, &RecordDialog::editFundName);
    connect(pB_comment, &QPushButton::clicked, this, &RecordDialog::editComment);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);
    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &RecordDialog::changeCurrent);
}

RecordDialog::~RecordDialog()
{
    delete m_model;
    delete m_proxyModel;
    delete pB_comment;
    delete pB_fundTitle;
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

void RecordDialog::changeCurrent(const QModelIndex &current, const QModelIndex &)
{
    RecordModel::RecordNode *node = static_cast<RecordModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    insertShortcut->setEnabled(node == nullptr || node->level != RecordModel::RecordLevel);
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);

    pB_fundTitle->setEnabled(current.isValid() && (node != nullptr && node->level == RecordModel::FundLevel));
    pB_comment->setEnabled(current.isValid());

    setInfoText();
}

void RecordDialog::setInfoText()
{
    QModelIndex currentIndex = ui->tV_itemView->currentIndex();

    ui->label_info->setText(
                currentIndex.data(Qt::UserRole + 2).toString() +
                (currentIndex.data(Qt::UserRole + 2).isNull() ? "" : "<br/><br/>") +
                "<i style=color:grey>" +
                currentIndex.data(Qt::UserRole + 1).toString() +
                "</i>");
}

void RecordDialog::edit()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    ui->tV_itemView->edit(index);
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

void RecordDialog::editComment()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    QInputDialog inputDialog;
    inputDialog.setWindowTitle(tr("Comment"));
    inputDialog.setLabelText(tr("Enter comment:"));
    inputDialog.setTextValue(index.data(Qt::UserRole + 1).toString());
    inputDialog.setTextEchoMode(QLineEdit::Normal);

    inputDialog.setMinimumWidth(480);
    inputDialog.resize(inputDialog.size());

    bool res = inputDialog.exec();

    if (res && !inputDialog.textValue().isEmpty()) {
        bool set;
        set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()), inputDialog.textValue(), Qt::UserRole + 1);

        if(set) {
            setInfoText();
        } else {
            bool tooLong = false;
            if(inputDialog.textValue().length() >= 255) {
                tooLong = true;
            }
            QMessageBox::warning(this,
                    tr("Comment"),
                    tr("Could not set the comment data.") + (tooLong ? tr(" Too long.") : ""),
                    QMessageBox::Ok);
        }
    }
}

void RecordDialog::editFundName()
{
    QModelIndex index = ui->tV_itemView->currentIndex();

    QInputDialog inputDialog;
    inputDialog.setWindowTitle(tr("Fund name"));
    inputDialog.setLabelText(tr("Enter fund name:"));
    inputDialog.setTextValue(index.data(Qt::UserRole + 2).toString());
    inputDialog.setTextEchoMode(QLineEdit::Normal);

    inputDialog.setMinimumWidth(480);
    inputDialog.resize(inputDialog.size());

    bool res = inputDialog.exec();

    if (res && !inputDialog.textValue().isEmpty()) {
        bool set;
        set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(ui->tV_itemView->currentIndex()), inputDialog.textValue(), Qt::UserRole + 2);

        if(set) {
            setInfoText();
        } else {
            bool tooLong = false;
            if(inputDialog.textValue().length() >= 255) {
                tooLong = true;
            }
            QMessageBox::warning(this,
                    tr("Fund name"),
                    tr("Could not set the fund name.") + (tooLong ? tr(" Too long.") : ""),
                    QMessageBox::Ok);
        }
    }
}

void RecordDialog::refresh()
{
    ui->tV_itemView->selectionModel()->clearCurrentIndex();

    m_proxyModel->invalidate();
    m_model->select();
}

void RecordDialog::remove()
{
    QModelIndex index = ui->tV_itemView->currentIndex();
    QModelIndex parent = m_proxyModel->parent(index);

    int res = QMessageBox::critical(this,
        tr("Deleting item"),
        tr("Are you shure that you want to delete this item?"),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        bool remove = m_proxyModel->removeRow(index.row(), parent);
        if (!remove) {
            QMessageBox::warning(this,
                    tr("Deleting item"),
                    tr("Could not remove the item."),
                    QMessageBox::Ok);
        }
    }
}
