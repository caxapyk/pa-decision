#include "inventorydetailsdialog.h"
#include "ui_inventorydetailsdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlTableModel>

InventoryDetailsDialog::InventoryDetailsDialog(const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDetailsDialog)
{
    m_id = id;
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &InventoryDetailsDialog::reject);
}

InventoryDetailsDialog::~InventoryDetailsDialog()
{
    delete ui;
    delete m_mapper;
}

int InventoryDetailsDialog::exec()
{
    QSqlTableModel model;
    model.setTable("pad_inventory");
    model.setFilter("id=" + m_id.toString());
    model.select();

    QModelIndex index = model.index(0, 0);

    if (index.isValid()) {
        setWindowTitle(tr("Edit inventory"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(&model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 2);
        m_mapper->addMapping(ui->lE_volume, 3);
        m_mapper->addMapping(ui->lE_title, 4);
        m_mapper->addMapping(ui->lE_comment, 5);
        m_mapper->setCurrentIndex(index.row());

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
            if(m_mapper->submit()) {
                accept();
            } else {
                QMessageBox::critical(this,
                                      tr("Fund"),
                                      tr("Could not save inventory"),
                                      QMessageBox::Ok);
            }
        });

    }

    return QDialog::exec();
}

QString InventoryDetailsDialog::getNumber() const
{
    return ui->lE_number->text();
}

QString InventoryDetailsDialog::getName() const
{
    return ui->lE_title->text();
}

void InventoryDetailsDialog::reject()
{
    bool commit = ui->lE_title->isModified() || ui->lE_number->isModified() || ui->lE_volume->isModified() || ui->lE_comment->isModified();
    if(commit) {
    int res = QMessageBox::critical(this,
        tr("Close without saving"),
        tr("Are you shure that you want to dicard all changes?"),
        QMessageBox::No | QMessageBox::Yes);

        if (res == QMessageBox::Yes) {
            QDialog::reject();
        }
    } else {
        QDialog::reject();
    }
}

