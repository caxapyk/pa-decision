#include "choicedialog.h"
#include "ui_choicedialog.h"

#include "models/recordmodel.h"
#include "widgets/basecontextmenu.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>

ChoiceDialog::ChoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceDialog)
{
    ui->setupUi(this);

    //m_tree = new CustomTreeView;
    //ui->hL_body->insertWidget(0, m_tree);

    /*connect(m_tree, &CustomTreeView::onInsert, this, &ChoiceDialog::insert);
    connect(m_tree, &CustomTreeView::onEdit, this,  &ChoiceDialog::edit);
    connect(m_tree, &CustomTreeView::onRemove, this,  &ChoiceDialog::remove);
    connect(m_tree, &CustomTreeView::onRefresh, this, &ChoiceDialog::refresh);*/

    setInfoIconVisible(false);
    ui->label_commentIcon->setVisible(false);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ChoiceDialog::accept);
}

ChoiceDialog::~ChoiceDialog()
{
    delete ui;
    delete m_treeView;
}

void ChoiceDialog::setTreeView(TreeView *tv)
{
    m_treeView = tv;
    ui->hL_body->insertWidget(0, m_treeView);
}

QBoxLayout *ChoiceDialog::buttonLayout()
{
    return ui->vL_buttonGroup;
}

void ChoiceDialog::setChoiceMode(bool ok)
{
    if(ok)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    choice_mode = ok;
};

void ChoiceDialog::clearInfoText()
{
    ui->label_info->clear();
}

/*void ChoiceDialog::setProxyModel(QSortFilterProxyModel *model)
{
    m_dialogProxyModel = model;

    connect(m_tree->selectionModel(), &QItemSelectionModel::currentChanged, this, &ChoiceDialog::onCurrentChanged);
}*/

void ChoiceDialog::setInfoText(const QString &text)
{
    ui->label_info->setText(text);
}

void ChoiceDialog::setInfoIconVisible(bool ok)
{
    ui->label_infoIcon->setVisible(ok);
}

void ChoiceDialog::onCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    m_choice = choice(current);

    QModelIndexList selected = m_treeView->selectionModel()->selectedRows();

    m_treeView->setInsertEnabled(true);
    m_treeView->setEditEnabled(!selected.isEmpty());
    m_treeView->setRemoveEnabled(!selected.isEmpty());
    m_treeView->setRefreshEnabled(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(choiceButtonEnabled());
}

/*void ChoiceDialog::edit()
{
    m_tree->edit(m_tree->currentIndex());
}*/

/*void ChoiceDialog::insert()
{
    bool insert = m_dialogProxyModel->sourceModel()->insertRow(0);

    if(insert) {
        m_dialogProxyModel->submit();
        for (int i = 0; i < m_dialogProxyModel->columnCount(); ++i) {
            m_tree->resizeColumnToContents(i);
        }

        m_tree->selectionModel()->clearCurrentIndex();

        QModelIndex topLeft = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, 0));
        QModelIndex bottomRight = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, m_dialogProxyModel->columnCount() - 1));

        QItemSelection selection(topLeft, bottomRight);
        m_tree->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
        m_tree->setCurrentIndex(topLeft);
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
}*/

/*void ChoiceDialog::refresh()
{
    if(m_dialogProxyModel != nullptr) {
        //m_dialogProxyModel->invalidate();
        //m_tree->sortByColumn(-1, Qt::AscendingOrder);

        RecordModel *recordmodel = dynamic_cast<RecordModel*>(m_dialogProxyModel->sourceModel());
        QSqlTableModel *qsqltablemodel = dynamic_cast<QSqlTableModel*>(m_dialogProxyModel->sourceModel());

        if(recordmodel) {
            recordmodel->select();
        } else if(qsqltablemodel) {
            qsqltablemodel->select();
        }
    }
}*/

/*void ChoiceDialog::remove()
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
}*/

void ChoiceDialog::accept()
{
    saveDialogState();
    QDialog::accept();
}

void ChoiceDialog::reject()
{
    saveDialogState();
    QDialog::reject();
}

void ChoiceDialog::closeEvent(QCloseEvent *event)
{
   saveDialogState();
   QDialog::closeEvent(event);
}
