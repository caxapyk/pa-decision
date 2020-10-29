#include "doctypedialog.h"
#include "ui_doctypedialog.h"

#include <QDebug>
#include <QMenu>

DoctypeDialog::DoctypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoctypeDialog)
{
    ui->setupUi(this);

    m_model = new QSqlTableModel;
    m_model->setTable("pad_doctype");
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_doctype->setModel(m_proxyModel);
    ui->tV_doctype->setContextMenuPolicy(Qt::CustomContextMenu);

    setupShortcuts();

    connect(ui->tV_doctype, &QMenu::customContextMenuRequested, this, &DoctypeDialog::contextMenu);
    connect(ui->tV_doctype->selectionModel(), &QItemSelectionModel::currentChanged, this, &DoctypeDialog::changeCurrent);
}

DoctypeDialog::~DoctypeDialog()
{
    delete ui;
    delete m_model;
    delete m_proxyModel;
}

void DoctypeDialog::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_doctype, nullptr, nullptr, Qt::WidgetShortcut);
    connect(insertShortcut, &QShortcut::activated, this, &DoctypeDialog::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_doctype, nullptr, nullptr, Qt::WidgetShortcut);
    connect(editShortcut, &QShortcut::activated, this, &DoctypeDialog::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_doctype, nullptr, nullptr, Qt::WidgetShortcut);
    connect(removeShortcut, &QShortcut::activated, this, &DoctypeDialog::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_doctype, nullptr, nullptr, Qt::WidgetShortcut);
    connect(refreshShortcut, &QShortcut::activated, this, &DoctypeDialog::refresh);
}

void DoctypeDialog::changeCurrent(const QModelIndex &current, const QModelIndex &)
{
    insertShortcut->setEnabled(!current.isValid());
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);
}

void DoctypeDialog::contextMenu(const QPoint &pos)
{
    QModelIndex currentIndex = ui->tV_doctype->indexAt(ui->tV_doctype->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_doctype->setCurrentIndex(currentIndex);

    QMenu menu;

    QAction insertAction(QIcon(":/icons/icons/add-16.png"), tr("New"));
    insertAction.setShortcut(insertShortcut->key());
    insertAction.setEnabled(insertShortcut->isEnabled());
    connect(&insertAction, &QAction::triggered, this, &DoctypeDialog::insert);
    menu.addAction(&insertAction);

    QAction editAction(QIcon(":/icons/icons/edit-16.png"), tr("Edit"));
    editAction.setShortcut(editShortcut->key());
    editAction.setEnabled(editShortcut->isEnabled());
    connect(&editAction, &QAction::triggered, this,  &DoctypeDialog::edit);
    menu.addAction(&editAction);

    QAction removeAction(QIcon(":/icons/icons/remove-16.png"), tr("Remove"));
    removeAction.setShortcut(removeShortcut->key());
    removeAction.setEnabled(removeShortcut->isEnabled());
    connect(&removeAction, &QAction::triggered, this,  &DoctypeDialog::remove);
    menu.addAction(&removeAction);

    menu.addSeparator();

    QAction refreshAction(QIcon(":/icons/icons/refresh-16.png"), tr("Refresh"));
    refreshAction.setShortcut(refreshShortcut->key());
    connect(&refreshAction, &QAction::triggered, this, &DoctypeDialog::refresh);

    menu.addAction(&refreshAction);

    menu.exec(QCursor().pos());
}

void DoctypeDialog::edit()
{

}

void DoctypeDialog::insert()
{

}

void DoctypeDialog::refresh()
{
    qDebug() << "refresh";
}

void DoctypeDialog::remove()
{

}

