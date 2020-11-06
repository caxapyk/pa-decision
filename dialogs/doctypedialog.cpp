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
    setWindowTitle(tr("Document types"));
    setInfoIconVisible();
    setComment(tr("Use color to highlight documents!"));

    m_model = new DocumentTypeModel;
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tV_itemView->resizeColumnToContents(0);
    ui->tV_itemView->setItemDelegateForColumn(1, new ColorPickerItemDelegate);

    setDialogModel(m_proxyModel);

    connect(ui->tV_itemView, &QMenu::customContextMenuRequested, this, &ReferenceDialog::contextMenu);
}

DoctypeDialog::~DoctypeDialog()
{
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

void DoctypeDialog::selected(const QModelIndex &current, const QModelIndex &)
{
    insertShortcut->setEnabled(!current.isValid());
    editShortcut->setEnabled(current.isValid());
    removeShortcut->setEnabled(current.isValid());
    refreshShortcut->setEnabled(true);
}

void DoctypeDialog::insert()
{
    bool insert = m_proxyModel->sourceModel()->insertRow(0);

    if(insert) {
        QModelIndex currentIndex = m_proxyModel->mapFromSource(
                    m_proxyModel->sourceModel()->index(0, 1));

        ui->tV_itemView->resizeColumnToContents(1);

        ui->tV_itemView->setCurrentIndex(currentIndex);
        ui->tV_itemView->scrollTo(currentIndex);
        ui->tV_itemView->edit(ui->tV_itemView->currentIndex());
    } else {
        QMessageBox::warning(this,
                tr("Creating items"),
                tr("Could not create item."),
                QMessageBox::Ok);
    }
}

