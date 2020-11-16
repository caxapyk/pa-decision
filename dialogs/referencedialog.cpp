#include "referencedialog.h"
#include "ui_referencedialog.h"

#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

ReferenceDialog::ReferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReferenceDialog)
{
    ui->setupUi(this);

    setInfoIconVisible(false);
    ui->label_commentIcon->setVisible(false);

    setupShortcuts();

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ReferenceDialog::accept);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ReferenceDialog::reject);
}

ReferenceDialog::~ReferenceDialog()
{
    delete ui;

    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
    delete refreshShortcut;

    delete pB_comment;
}

void ReferenceDialog::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &ReferenceDialog::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &ReferenceDialog::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &ReferenceDialog::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_itemView, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &ReferenceDialog::refresh);
}

void ReferenceDialog::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_itemView->indexAt(ui->tV_itemView->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_itemView->setCurrentIndex(currentIndex);

    CustomContextMenu menu(CustomContextMenu::All);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(insertShortcut->isEnabled());
    connect(insertAction, &QAction::triggered, this, &ReferenceDialog::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &ReferenceDialog::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &ReferenceDialog::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, &ReferenceDialog::refresh);

    menu.exec(QCursor().pos());
}

void ReferenceDialog::addCommentButton()
{
    pB_comment = new QPushButton(tr("Comment"));
    pB_comment->setIcon(QIcon(":/icons/icons/comment-16.png"));
    pB_comment->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_comment);

    connect(pB_comment, &QPushButton::clicked, this, &ReferenceDialog::editComment);
}

QVariant ReferenceDialog::inputDialog(const QString &title, const QString &label, const QVariant &value)
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

void ReferenceDialog::loadByAuthorityId(int id)
{
    ReferenceModel *model = dynamic_cast<ReferenceModel*>(m_dialogProxyModel->sourceModel());

    if(model) {
        clearInfoText();

        model->setAuthorityId(id);
        model->select();

        _selected(QModelIndex(), QModelIndex());
        selected(QModelIndex(), QModelIndex());
    }
}

void ReferenceDialog::clearComment()
{
    ui->label_comment->clear();
    ui->label_commentIcon->setVisible(false);
}

void ReferenceDialog::clearInfoText()
{
    ui->label_info->clear();
}

void ReferenceDialog::setDialogModel(QSortFilterProxyModel *model)
{
    m_dialogProxyModel = model;

    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ReferenceDialog::_selected);
    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ReferenceDialog::selected);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);
}

void ReferenceDialog::setComment(const QString &text)
{
    ui->label_commentIcon->setVisible(!text.isEmpty());
    ui->label_comment->setText(text);
}

void ReferenceDialog::setInfoText(const QString &text)
{
    ui->label_info->setText(text);
}

void ReferenceDialog::setInfoIconVisible(bool ok)
{
    ui->label_infoIcon->setVisible(ok);
}

void ReferenceDialog::_selected(const QModelIndex &current, const QModelIndex &)
{
    m_choice = choice(current);

    if(!current.isValid()) {
        clearInfoText();
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(choiceButtonEnabled());
}

void ReferenceDialog::edit()
{
    ui->tV_itemView->edit(ui->tV_itemView->currentIndex());
}

void ReferenceDialog::editComment()
{
    if(m_commentColumn) {
        QModelIndex index = ui->tV_itemView->currentIndex().siblingAtColumn(m_commentColumn);
        QString title = tr("Comment");

        QVariant value = inputDialog(title, tr("Enter comment"), index.data());

        if (value.isValid() && value != index.data()) {
            bool set = m_dialogProxyModel->sourceModel()->setData(m_dialogProxyModel->mapToSource(index), value);
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
}

void ReferenceDialog::insert()
{
    bool insert = m_dialogProxyModel->sourceModel()->insertRow(0);

    if(insert) {
        QModelIndex currentIndex = m_dialogProxyModel->mapFromSource(
                    m_dialogProxyModel->sourceModel()->index(0, 1));

        ui->tV_itemView->resizeColumnToContents(1);

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

void ReferenceDialog::refresh()
{
    if(m_dialogProxyModel != nullptr) {
        m_dialogProxyModel->invalidate();
        ui->tV_itemView->sortByColumn(-1, Qt::AscendingOrder);

        ReferenceModel *model = dynamic_cast<ReferenceModel*>(m_dialogProxyModel->sourceModel());

        if(model) {
            ui->tV_itemView->selectionModel()->clearCurrentIndex();
            model->select();

            _selected(QModelIndex(), QModelIndex());
            selected(QModelIndex(), QModelIndex());
        }
    }
}

void ReferenceDialog::remove()
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
            if (!remove) {
                QMessageBox::warning(this,
                        tr("Deleting item"),
                        tr("Could not remove the item."),
                        QMessageBox::Ok);
            }
        }
    }
}

void ReferenceDialog::accept()
{
    saveDialogState();
    QDialog::accept();
}

void ReferenceDialog::reject()
{
    saveDialogState();
    QDialog::reject();
}

void ReferenceDialog::closeEvent(QCloseEvent *event)
{
   saveDialogState();
   QDialog::closeEvent(event);
}
