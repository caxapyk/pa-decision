#include "subjectdialog.h"
#include "ui_referencedialog.h"

#include "application.h"

#include <QDebug>
#include <QMessageBox>
#include <QMenu>


SubjectDialog::SubjectDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Subjects"));

    ui->label_infoIcon->setVisible(true);
    setInfoText(tr("Use subjects for grouping desicions!"));

    m_model = new SubjectModel;
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(0);

    setDialogModel(m_proxyModel);
}

SubjectDialog::~SubjectDialog()
{
    saveDialogState();

    delete m_model;
    delete m_proxyModel;
}

void SubjectDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("SubjectDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("DoctypeDialog/tV_itemView").toByteArray());
}

void SubjectDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("SubjectDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}
