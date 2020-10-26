#include "recorddialog.h"
#include "ui_recorddialog.h"

RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);

    m_model = new RecordTreeModel;
    m_model->select();

    m_proxy = new RecordProxyModel;
    m_proxy->setSourceModel(m_model);
    m_proxy->setSortRole(Qt::AscendingOrder);

    ui->tV_record->setModel(m_proxy);
}

RecordDialog::~RecordDialog()
{
    delete ui;
    delete m_model;
    delete m_proxy;
}
