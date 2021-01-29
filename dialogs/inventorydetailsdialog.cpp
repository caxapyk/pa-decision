#include "inventorydetailsdialog.h"
#include "ui_inventorydetailsdialog.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QRegExpValidator>

InventoryDetailsDialog::InventoryDetailsDialog(const QVariant &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDetailsDialog)
{
    m_id = id;
    ui->setupUi(this);

    ui->lE_volume->setValidator(new QRegExpValidator(QRegExp("\\d+")));

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &InventoryDetailsDialog::reject);
}

InventoryDetailsDialog::~InventoryDetailsDialog()
{
    delete ui;
    delete m_mapper;
}

int InventoryDetailsDialog::exec()
{

    m_model.setTable("pad_inventory");
    m_model.setFilter("id=" + m_id.toString());
    m_model.select();

    if (m_model.rowCount() > 0) {
        setWindowTitle(tr("Inventory details"));

        m_mapper = new QDataWidgetMapper;
        m_mapper->setModel(&m_model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

        m_mapper->addMapping(ui->lE_number, 2);
        m_mapper->addMapping(ui->lE_volume, 3);
        m_mapper->addMapping(ui->lE_title, 4);
        m_mapper->addMapping(ui->lE_comment, 5);
        m_mapper->setCurrentIndex(0);

        connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, [=] {
            if(!validate())
                return;

            if(m_mapper->submit()) {
                accept();
            } else {
                QMessageBox::critical(this,
                                      tr("Inventory details"),
                                      tr("Could not save data."),
                                      QMessageBox::Ok);
            }
        });

    }

    return QDialog::exec();
}

bool InventoryDetailsDialog::validate()
{
    if(ui->lE_volume->text().isEmpty()) {
        m_mapper->removeMapping(ui->lE_volume);
        m_model.setData(m_model.index(0, 3), QVariant());
    }

    if(ui->lE_number->text().isEmpty() || ui->lE_title->text().isEmpty()) {
        QMessageBox::critical(this,
                              tr("Inventory details"),
                              tr("Fill all required fields (*)."),
                              QMessageBox::Ok);
        return false;
    }

    return true;
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

