#include "decisionnewdialog.h"
#include "ui_decisionbasedialog.h"

#include "models/decisionmodel.h"

#include <QDebug>
#include <QMessageBox>

DecisionNewDialog::DecisionNewDialog(QWidget *parent) :
    DecisionBaseDialog(parent)
{
    setWindowTitle(tr("New decision"));
    ui->tabWidget->setDisabled(true);

    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &DecisionNewDialog::insert);
}

DecisionNewDialog::~DecisionNewDialog()
{
}

void DecisionNewDialog::setAuthorityId(int id)
{
    if(setChosenId(ui->cB_authority, id, 2)) {
        ui->cB_authority->setDisabled(true);
    }
}

void DecisionNewDialog::insert()
{
    /*if(!validate()) {
        QMessageBox::critical(this,
                              tr("New protocol"),
                              tr("Could not create new protocol - check all fields are correct."),
                              QMessageBox::Ok);

        return;
    }

    DecisionModel *m = dynamic_cast<DecisionModel*>(model());
    if(m) {
        bool insert = m->save(
                    recordModel()->index(ui->cB_record->currentIndex(), 0).data(),
                    authorityModel()->index(ui->cB_authority->currentIndex(), 2).data(),
                    documentTypeModel()->index(ui->cB_doctype->currentIndex(), 0).data(),
                    protocolModel()->index(ui->cB_protocol->currentIndex(), 0).data(),
                    ui->dE_date->date(),
                    ui->lE_number->text(),
                    ui->lE_title->text(),
                    ui->cB_access->currentIndex(),
                    ui->tE_content->toHtml(),
                    ui->lE_comment->text(),
                    QVariant()
                    );
        if(insert) {
            qDebug() << "saved" << m->lastInsertId();
            accept();
        } else {
            QMessageBox::critical(this,
                                  tr("New decision"),
                                  tr("Could not create new decision"),
                                  QMessageBox::Ok);
        }
    }*/
}
