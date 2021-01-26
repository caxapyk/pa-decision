#include "doctypedialog.h"

#include "application.h"
#include "models/colorpickeritemdelegate.h"

#include <QDebug>
#include <QHeaderView>

DoctypeDialog::DoctypeDialog(QWidget *parent) :
    TreeDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Document types"));
    setWindowIcon(QIcon(":/icons/icons/doctype-16.png"));

    setInfoIconVisible();
    setInfoText(tr("Use color to highlight documents!"));

    m_model = new DocumentTypeModel;
    m_model->setTable("pad_doctype");
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    m_tree->setModel(m_proxyModel);
    m_tree->hideColumn(0);
    m_tree->resizeColumnToContents(1);
    m_tree->setItemDelegateForColumn(2, new ColorPickerItemDelegate);

    setProxyModel(m_proxyModel);
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
    m_tree->header()->restoreState(settings->value("DoctypeDialog/tV_itemView").toByteArray());
}

void DoctypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DoctypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", m_tree->header()->saveState());
    settings->endGroup();
}

bool DoctypeDialog::choiceButtonEnabled()
{
    return !isChoiceMode() || !m_tree->selectionModel()->selection().isEmpty();
}
