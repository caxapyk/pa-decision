#include "recorddialog.h"
#include "ui_recorddialog.h"

#include <QDebug>
#include <QMenu>

RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);

    m_model = new RecordTreeModel;
    m_model->select();

    m_proxy = new RecordProxyModel;
    m_proxy->setSourceModel(m_model);

    ui->tV_record->setModel(m_model);
    ui->tV_record->setContextMenuPolicy(Qt::CustomContextMenu);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_record, nullptr, nullptr, Qt::WidgetShortcut);

    connect(ui->pB_create, &QPushButton::clicked, this, &RecordDialog::insert);
    connect(ui->pB_edit, &QPushButton::clicked, this, &RecordDialog::edit);
    connect(ui->pB_remove, &QPushButton::clicked, this, &RecordDialog::remove);
    connect(ui->pB_title, &QPushButton::clicked, this, &RecordDialog::title);

    connect(ui->tV_record, &QMenu::customContextMenuRequested, this, &RecordDialog::contextMenu);
}

RecordDialog::~RecordDialog()
{
    delete ui;
    delete m_model;
    delete m_proxy;
    delete m_refreshShortcut;
}

void RecordDialog::contextMenu(const QPoint &)
{
    QMenu menu;

    QAction refreshAction(QIcon(":/icons/icons/refresh-16.png"), tr("Refresh"));
    refreshAction.setShortcut(m_refreshShortcut->key());
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
    //m_proxy->invalidate();
    m_model->select();
}

void RecordDialog::remove()
{

}

void RecordDialog::title()
{

}
