#include "recorddialog.h"
#include "ui_recorddialog.h"

#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);

    m_model = new RecordTreeModel;
    m_model->select();

    m_proxyModel = new RecordProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_record->setModel(m_proxyModel);
    ui->tV_record->setContextMenuPolicy(Qt::CustomContextMenu);

    setupShortcuts();

    connect(ui->tV_record, &QMenu::customContextMenuRequested, this, &RecordDialog::contextMenu);
    connect(ui->tV_record->selectionModel(), &QItemSelectionModel::currentChanged, this, &RecordDialog::changeCurrent);
    connect(ui->pB_title, &QPushButton::clicked, this, &RecordDialog::editFundName);
    connect(ui->pB_comment, &QPushButton::clicked, this, &RecordDialog::editComment);
}

RecordDialog::~RecordDialog()
{
    delete ui;
    delete m_model;
    delete m_proxyModel;
    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
    delete refreshShortcut;
}

void RecordDialog::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_record, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &RecordDialog::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_record, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &RecordDialog::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_record, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &RecordDialog::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_record, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &RecordDialog::refresh);
}

void RecordDialog::changeCurrent(const QModelIndex &current, const QModelIndex &)
{
    RecordTreeModel::RecordNode *node = static_cast<RecordTreeModel::RecordNode*>(m_proxyModel->mapToSource(current).internalPointer());

    insertShortcut->setEnabled(node == nullptr || node->level != RecordTreeModel::RecordLevel);
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);

    ui->pB_title->setEnabled(current.isValid() && (node != nullptr && node->level == RecordTreeModel::FundLevel));
    ui->pB_comment->setEnabled(current.isValid());

    setInfoText();
}

void RecordDialog::setInfoText()
{
    QModelIndex currentIndex = ui->tV_record->currentIndex();

    ui->label_info->setText(
                currentIndex.data(Qt::UserRole + 2).toString() +
                (currentIndex.data(Qt::UserRole + 2).isNull() ? "" : "<br/><br/>") +
                "<i style=color:grey>" +
                currentIndex.data(Qt::UserRole + 1).toString() +
                "</i>");
}

void RecordDialog::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_record->indexAt(ui->tV_record->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_record->setCurrentIndex(currentIndex);

    QString item;

    if(!currentIndex.isValid()) {
        item = tr("fund");
    } else {
        RecordTreeModel::RecordNode *node = static_cast<RecordTreeModel::RecordNode*>(m_proxyModel->mapToSource(currentIndex).internalPointer());
        switch (node->level) {
        case RecordTreeModel::FundLevel:
            item = tr("inventory");
            break;
        case RecordTreeModel::InventoryLevel:
            item = tr("record");
            break;
        }
    }

    CustomContextMenu menu(CustomContextMenu::All);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setText(insertAction->text() + tr(" %1").arg(item));
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, &RecordDialog::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &RecordDialog::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &RecordDialog::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, &RecordDialog::refresh);

    menu.exec(QCursor().pos());
}

void RecordDialog::insert()
{
    QModelIndex proxyParent = ui->tV_record->currentIndex();
    QModelIndex sourceParent = m_proxyModel->mapToSource(proxyParent);

    if(!ui->tV_record->isExpanded(proxyParent)) {
        ui->tV_record->expand(proxyParent);
    }

    int v = m_proxyModel->sourceModel()->rowCount(sourceParent);

    bool insert = m_proxyModel->sourceModel()->insertRow(v, sourceParent);

    if(insert) {
        QModelIndex currentIndex = m_proxyModel->mapFromSource(m_proxyModel->sourceModel()->index(v, 0, sourceParent));

        ui->tV_record->setCurrentIndex(currentIndex);
        ui->tV_record->scrollTo(currentIndex);
        ui->tV_record->edit(currentIndex);
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void RecordDialog::edit()
{
    QModelIndex index = ui->tV_record->currentIndex();

    ui->tV_record->edit(index);
}

void RecordDialog::editComment()
{
    QModelIndex index = ui->tV_record->currentIndex();

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
        set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(ui->tV_record->currentIndex()), inputDialog.textValue(), Qt::UserRole + 1);

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
    QModelIndex index = ui->tV_record->currentIndex();

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
        set = m_proxyModel->sourceModel()->setData(m_proxyModel->mapToSource(ui->tV_record->currentIndex()), inputDialog.textValue(), Qt::UserRole + 2);

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
    ui->tV_record->selectionModel()->clearCurrentIndex();

    m_proxyModel->invalidate();
    m_model->select();
}

void RecordDialog::remove()
{
    QModelIndex index = ui->tV_record->currentIndex();
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

void RecordDialog::title()
{

}
