#include "recorddialog.h"
#include "ui_recorddialog.h"

#include <QDebug>
#include <QItemSelection>
#include <QMenu>

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

    ui->pB_title->setEnabled(current.isValid() && (node != nullptr && node->level == RecordTreeModel::RecordLevel));
}

void RecordDialog::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_record->indexAt(ui->tV_record->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_record->setCurrentIndex(currentIndex);

    QMenu menu;

    QAction insertAction(QIcon(":/icons/icons/add-16.png"), tr("New"));
    insertAction.setShortcut(insertShortcut->key());
    insertAction.setEnabled(insertShortcut->isEnabled());
    connect(&insertAction, &QAction::triggered, this, &RecordDialog::insert);
    menu.addAction(&insertAction);

    QAction editAction(QIcon(":/icons/icons/edit-16.png"), tr("Edit"));
    editAction.setShortcut(editShortcut->key());
    editAction.setEnabled(editShortcut->isEnabled());
    connect(&editAction, &QAction::triggered, this,  &RecordDialog::edit);
    menu.addAction(&editAction);

    QAction removeAction(QIcon(":/icons/icons/remove-16.png"), tr("Remove"));
    removeAction.setShortcut(removeShortcut->key());
    removeAction.setEnabled(removeShortcut->isEnabled());
    connect(&removeAction, &QAction::triggered, this,  &RecordDialog::remove);
    menu.addAction(&removeAction);

    menu.addSeparator();


    QAction refreshAction(QIcon(":/icons/icons/refresh-16.png"), tr("Refresh"));
    refreshAction.setShortcut(refreshShortcut->key());
    connect(&refreshAction, &QAction::triggered, this, &RecordDialog::refresh);

    menu.addAction(&refreshAction);

    menu.exec(QCursor().pos());
}

void RecordDialog::edit()
{

}

void RecordDialog::insert()
{

}

void RecordDialog::refresh()
{
    ui->tV_record->selectionModel()->clearCurrentIndex();

    m_proxyModel->invalidate();
    m_model->select();
}

void RecordDialog::remove()
{

}

void RecordDialog::title()
{

}
