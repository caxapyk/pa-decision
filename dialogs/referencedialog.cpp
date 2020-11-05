#include "referencedialog.h"
#include "ui_referencedialog.h"

#include "widgets/customcontextmenu.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

ReferenceDialog::ReferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReferenceDialog)
{
    ui->setupUi(this);

    restoreDialogState();
    setupShortcuts();

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ReferenceDialog::accept);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ReferenceDialog::reject);

    connect(ui->tV_itemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ReferenceDialog::choice);
}

ReferenceDialog::~ReferenceDialog()
{
    delete ui;

    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
    delete refreshShortcut;

    delete pB_comment;

    delete m_dialogProxyModel;
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
    pB_comment->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_comment);
}

void ReferenceDialog::editComment()
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
        set = m_dialogProxyModel->sourceModel()->setData(m_dialogProxyModel->mapToSource(ui->tV_itemView->currentIndex()), inputDialog.textValue(), Qt::UserRole + 1);

        if(set) {
            //setInfoText();
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
