#include "choicedialog.h"
#include "ui_choicedialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>

ChoiceDialog::ChoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceDialog)
{
    ui->setupUi(this);

    setInfoIconVisible(false);
    ui->label_commentIcon->setVisible(false);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ChoiceDialog::accept);
}

ChoiceDialog::~ChoiceDialog()
{
    delete ui;
}

void ChoiceDialog::setTreeView(TreeView *tv)
{
    m_view = tv;
    ui->hL_body->insertWidget(0, m_view);

    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ChoiceDialog::selectionChanged, Qt::UniqueConnection);
}

QBoxLayout *ChoiceDialog::buttonLayout()
{
    return ui->vL_buttonGroup;
}

void ChoiceDialog::setChoiceMode(bool ok)
{
    if(ok)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    choice_mode = ok;
}

void ChoiceDialog::clearInfoText()
{
    ui->label_info->clear();
}

void ChoiceDialog::setInfoText(const QString &text)
{
    ui->label_info->setText(text);
}

void ChoiceDialog::setInfoIconVisible(bool ok)
{
    ui->label_infoIcon->setVisible(ok);
}

void ChoiceDialog::selectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    m_choice = choice(selected.isEmpty() ? QModelIndex() : selected.indexes().last());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(choiceButtonEnabled());
}

void ChoiceDialog::accept()
{
    saveDialogState();
    QDialog::accept();
}

void ChoiceDialog::reject()
{
    saveDialogState();
    QDialog::reject();
}

void ChoiceDialog::closeEvent(QCloseEvent *event)
{
   saveDialogState();
   QDialog::closeEvent(event);
}
