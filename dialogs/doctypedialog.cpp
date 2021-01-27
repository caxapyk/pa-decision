#include "doctypedialog.h"

#include "application.h"
#include "models/colorpickeritemdelegate.h"

#include <QDebug>
#include <QHeaderView>

DoctypeDialog::DoctypeDialog(QWidget *parent) :
    ChoiceDialog(parent)
{
    m_view = new DocumentTypeTreeView;
    setTreeView(m_view);

    restoreDialogState();

    setWindowTitle(tr("Document types"));
    setWindowIcon(QIcon(":/icons/icons/doctype-16.png"));

    setInfoIconVisible();
    setInfoText(tr("Use color to highlight documents!"));
}

DoctypeDialog::~DoctypeDialog()
{
    saveDialogState();

    delete m_view;
}

void DoctypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DoctypeDialog/geometry").toByteArray());
}

void DoctypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DoctypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}
