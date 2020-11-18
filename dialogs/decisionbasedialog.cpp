#include "decisionbasedialog.h"
#include "ui_decisionbasedialog.h"

#include "dialogs/recorddialog.h"

#include <QDebug>

DecisionBaseDialog::DecisionBaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecisionBaseDialog)
{
    ui->setupUi(this);

    initialize();
}

DecisionBaseDialog::~DecisionBaseDialog()
{
    delete ui;

    delete m_decisionModel;
    delete m_authorityModel;
    delete m_doctypeModel;
}


void DecisionBaseDialog::initialize()
{
    m_decisionModel = new DecisionModel;

    m_authorityModel = new AuthorityFlatModel;
    m_authorityModel->select();
    ui->cB_authority->setModel(m_authorityModel);

    m_doctypeModel = new DocumentTypeModel;
    m_doctypeModel->select();
    ui->cB_doctype->setModel(m_doctypeModel);
    ui->cB_doctype->setModelColumn(0);

    connect(ui->pB_record, &QPushButton::clicked, this, &DecisionBaseDialog::chooseRecord);
}

void DecisionBaseDialog::chooseRecord()
{
    RecordDialog dialog;
    dialog.setChoiceMode();

    int res = dialog.exec();

    if(res == RecordDialog::Accepted) {
        int choice = dialog.currentChoice();
        qDebug() << "selected id:" << choice;
    }
}

void DecisionBaseDialog::setChosenId(QComboBox *cb, int id, int column)
{
    QAbstractItemModel *model = cb->model();
    for (int i = 0; i < model->rowCount(); ++i) {
        qDebug() << model->index(i, column).data().toInt() << "==" << id;
        if(model->index(i, 0).data().toInt() == id) {
            cb->setCurrentIndex(i);
            break;
        }
    }
}
