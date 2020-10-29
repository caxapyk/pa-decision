#include "doctypedialog.h"
#include "ui_doctypedialog.h"

#include <QDebug>
#include <QMenu>

DoctypeDialog::DoctypeDialog(QWidget *parent) :
    ReferenceDialogInterface(parent),
    ui(new Ui::DoctypeDialog)
{
    ui->setupUi(this);

    actions = new StandartViewActions;
    actions->setView(ui->tV_doctype);
    actions->setCrudWidget(this);
    actions->bindShortcuts();

    m_model = new QSqlTableModel;
    m_model->setTable("pad_doctype");
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_doctype->setModel(m_proxyModel);
    ui->tV_doctype->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_doctype, &QMenu::customContextMenuRequested, this, &DoctypeDialog::contextMenu);
    connect(ui->tV_doctype->selectionModel(), &QItemSelectionModel::currentChanged, this, &DoctypeDialog::changeCurrent);

    connect(actions->insertShortcut(), &QShortcut::activated, this, &DoctypeDialog::insert);
    connect(actions->editShortcut(), &QShortcut::activated, this, &DoctypeDialog::edit);
    connect(actions->removeShortcut(), &QShortcut::activated, this, &DoctypeDialog::remove);
    connect(actions->refreshShortcut(), &QShortcut::activated, this, &DoctypeDialog::refresh);
}

DoctypeDialog::~DoctypeDialog()
{
    delete ui;
    delete m_model;
    delete m_proxyModel;
    delete actions;
}

void DoctypeDialog::changeCurrent(const QModelIndex &current, const QModelIndex &)
{

}

void DoctypeDialog::contextMenu(const QPoint &pos)
{

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

