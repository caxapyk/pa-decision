#include "treedialog.h"
#include "ui_treedialog.h"

#include "models/recordmodel.h"
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

    m_tree = new CustomTreeView;
    ui->hL_body->insertWidget(0, m_tree);

    connect(m_tree, &CustomTreeView::onInsert, this, &TreeDialog::insert);
    connect(m_tree, &CustomTreeView::onEdit, this,  &TreeDialog::edit);
    connect(m_tree, &CustomTreeView::onRemove, this,  &TreeDialog::remove);
    connect(m_tree, &CustomTreeView::onRefresh, this, &TreeDialog::refresh);

    setInfoIconVisible(false);
    ui->label_commentIcon->setVisible(false);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &TreeDialog::accept);
}

TreeDialog::~TreeDialog()
{
    delete ui;
}

QBoxLayout *TreeDialog::buttonLayout()
{
    return ui->vL_buttonGroup;
}

void TreeDialog::contextMenu(CustomContextMenu &menu)
{
    QModelIndex currentIndex = m_tree->indexAt(m_tree->viewport()->mapFromGlobal(QCursor().pos()));
    m_tree->setCurrentIndex(currentIndex);

    menu.exec(QCursor().pos());
}

void TreeDialog::clearSelection()
{
    _selected(QItemSelection(), QItemSelection());
    selected(QItemSelection(), QItemSelection());

    m_tree->setCurrentIndex(QModelIndex());
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

void TreeDialog::setChoiceMode(bool ok)
{
    if(ok)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    choice_mode = ok;
};

void TreeDialog::clearInfoText()
{
    ui->label_info->clear();
}

void TreeDialog::setProxyModel(QSortFilterProxyModel *model)
{
    m_dialogProxyModel = model;

    connect(m_tree->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TreeDialog::_selected);
    connect(m_tree->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TreeDialog::selected);

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
    m_tree->setInsertEnabled(true);
    m_tree->setEditEnabled(!selected.isEmpty());
    m_tree->setRemoveEnabled(!selected.isEmpty());
    m_tree->setRefreshEnabled(true);
}

void TreeDialog::edit()
{
    m_tree->edit(m_tree->currentIndex());
}

void TreeDialog::insert()
{
    bool insert = m_dialogProxyModel->sourceModel()->insertRow(0);

    if(insert) {
        m_dialogProxyModel->submit();
        for (int i = 0; i < m_dialogProxyModel->columnCount(); ++i) {
            m_tree->resizeColumnToContents(i);
        }

        clearSelection();

        QModelIndex topLeft = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, 0));
        QModelIndex bottomRight = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, m_dialogProxyModel->columnCount() - 1));

        QItemSelection selection(topLeft, bottomRight);
        m_tree->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
        m_tree->scrollTo(topLeft);

        for (int i=0; i < m_dialogProxyModel->columnCount(); ++i) {
            if(m_tree->isColumnHidden(i) == false) {
                QModelIndex section = m_dialogProxyModel->index(topLeft.row(), i);
                m_tree->edit(section);
            }
        }
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
        //m_tree->sortByColumn(-1, Qt::AscendingOrder);

        RecordModel *recordmodel = dynamic_cast<RecordModel*>(m_dialogProxyModel->sourceModel());
        QSqlTableModel *qsqltablemodel = dynamic_cast<QSqlTableModel*>(m_dialogProxyModel->sourceModel());

        if(recordmodel) {
            recordmodel->select();
        } else if(qsqltablemodel) {
            qsqltablemodel->select();
        }

        m_tree->selectionModel()->clearCurrentIndex();
        clearSelection();
    }
}

void TreeDialog::remove()
{
    if(m_dialogProxyModel != nullptr) {
        QModelIndex index = m_tree->currentIndex();
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
