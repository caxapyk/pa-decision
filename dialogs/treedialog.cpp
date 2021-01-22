#include "treedialog.h"
#include "ui_treedialog.h"

#include "utils/customcontextmenu.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>

TreeDialog::TreeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TreeDialog)
{
    ui->setupUi(this);

    setInfoIconVisible(false);
    ui->label_commentIcon->setVisible(false);

    setupShortcuts();

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &TreeDialog::accept);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &TreeDialog::reject);
}

TreeDialog::~TreeDialog()
{
    delete ui;

    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
    delete refreshShortcut;
}

void TreeDialog::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &TreeDialog::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &TreeDialog::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &TreeDialog::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &TreeDialog::refresh);
}

void TreeDialog::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_itemView->indexAt(ui->tV_itemView->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_itemView->setCurrentIndex(currentIndex);

    CustomContextMenu menu(CustomContextMenu::All);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, &TreeDialog::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &TreeDialog::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &TreeDialog::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, &TreeDialog::refresh);

    menu.exec(QCursor().pos());
}

void TreeDialog::clearSelection()
{
    _selected(QItemSelection(), QItemSelection());
    selected(QItemSelection(), QItemSelection());

    ui->tV_itemView->setCurrentIndex(QModelIndex());
}


QVariant TreeDialog::inputDialog(const QString &title, const QString &label, const QVariant &value)
{
    QInputDialog inputDialog;
    inputDialog.setWindowTitle(title);
    inputDialog.setLabelText(label);
    inputDialog.setTextValue(value.toString());
    inputDialog.setTextEchoMode(QLineEdit::Normal);

    inputDialog.setMinimumWidth(480);
    inputDialog.resize(inputDialog.size());

    inputDialog.exec();

    return QVariant(inputDialog.textValue());
}

void TreeDialog::clearInfoText()
{
    ui->label_info->clear();
}

void TreeDialog::setProxyModel(QSortFilterProxyModel *model)
{
    m_dialogProxyModel = model;

    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TreeDialog::_selected);
    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TreeDialog::selected);

    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &TreeDialog::contextMenu);

    clearSelection();
}

void TreeDialog::setInfoText(const QString &text)
{
    ui->label_info->setText(text);
}

void TreeDialog::setInfoIconVisible(bool ok)
{
    ui->label_infoIcon->setVisible(ok);
}

void TreeDialog::_selected(const QItemSelection &selected, const QItemSelection &)
{
    m_choice = choice(selected);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(choiceButtonEnabled());
}

void TreeDialog::selected(const QItemSelection &selected, const QItemSelection &)
{
    insertShortcut->setEnabled(true);
    editShortcut->setEnabled(!selected.isEmpty());
    removeShortcut->setEnabled(!selected.isEmpty());
    refreshShortcut->setEnabled(true);
}

void TreeDialog::edit()
{
    ui->tV_itemView->edit(ui->tV_itemView->currentIndex());
}

void TreeDialog::insert()
{
    //QSqlTableModel *qsqltablemodel = dynamic_cast<QSqlTableModel*>(m_dialogProxyModel->sourceModel());
    bool insert = m_dialogProxyModel->sourceModel()->insertRow(0);

    if(insert) {
        m_dialogProxyModel->submit();
        for (int i = 0; i < m_dialogProxyModel->columnCount(); ++i) {
            ui->tV_itemView->resizeColumnToContents(i);
        }

        QModelIndex topLeft = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, 0));
        QModelIndex bottomRight = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, m_dialogProxyModel->columnCount() - 1));

        QItemSelection selection(topLeft, bottomRight);
        ui->tV_itemView->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
        ui->tV_itemView->scrollTo(topLeft);
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

void TreeDialog::refresh()
{
    if(m_dialogProxyModel != nullptr) {
        m_dialogProxyModel->invalidate();
        //ui->tV_itemView->sortByColumn(-1, Qt::AscendingOrder);

        SqlBaseModel *sqlbasemodel = dynamic_cast<SqlBaseModel*>(m_dialogProxyModel->sourceModel());
        QSqlTableModel *qsqltablemodel = dynamic_cast<QSqlTableModel*>(m_dialogProxyModel->sourceModel());

        if(sqlbasemodel) {
            sqlbasemodel->select();
        } else if(qsqltablemodel) {
            qsqltablemodel->select();
        }

        ui->tV_itemView->selectionModel()->clearCurrentIndex();
        clearSelection();
    }
}

void TreeDialog::remove()
{
    if(m_dialogProxyModel != nullptr) {
        QModelIndex index = ui->tV_itemView->currentIndex();
        QModelIndex parent = m_dialogProxyModel->parent(index);

        int res = QMessageBox::critical(this,
            tr("Deleting item"),
            tr("Are you shure that you want to delete this item?"),
            QMessageBox::No | QMessageBox::Yes);

        if (res == QMessageBox::Yes) {
            bool remove = m_dialogProxyModel->removeRow(index.row(), parent);
            if (remove) {
                // refresh QSqlTableModel
                if(dynamic_cast<QSqlTableModel*>(m_dialogProxyModel->sourceModel()))
                    refresh();
            } else {
                QMessageBox::warning(this,
                        tr("Deleting item"),
                        tr("Could not remove the item."),
                        QMessageBox::Ok);
            }
        }
    }
}

void TreeDialog::accept()
{
    saveDialogState();
    QDialog::accept();
}

void TreeDialog::reject()
{
    saveDialogState();
    QDialog::reject();
}

void TreeDialog::closeEvent(QCloseEvent *event)
{
   saveDialogState();
   QDialog::closeEvent(event);
}
