#include "doctypedialog.h"
#include "ui_referencedialog.h"

#include "application.h"
#include "models/colorpickeritemdelegate.h"

#include <QDebug>
#include <QMessageBox>
#include <QMenu>

DoctypeDialog::DoctypeDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Document types"));
    setInfoIconVisible();
    setInfoText(tr("Use color to highlight documents!"));

    m_model = new DocumentTypeModel;
    m_model->setTable("pad_doctype");
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(0);
    ui->tV_itemView->resizeColumnToContents(1);
    ui->tV_itemView->setItemDelegateForColumn(2, new ColorPickerItemDelegate);

    setDialogModel(m_proxyModel);
}

DoctypeDialog::~DoctypeDialog()
{
    saveDialogState();

    delete m_model;
    delete m_proxyModel;
}

void DoctypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DoctypeDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("DoctypeDialog/tV_itemView").toByteArray());
}

void DoctypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DoctypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}
