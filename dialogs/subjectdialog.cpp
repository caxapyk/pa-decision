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
    setInfoText(tr("Use subjects for grouping desicions by members type!"));
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

void SubjectDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    insertShortcut->setEnabled(!current.isValid());
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);
}
