#include "documenttypedialog.h"

#include "application.h"
#include "models/colorpickeritemdelegate.h"

#include <QDebug>
#include <QHeaderView>

DocumentTypeDialog::DocumentTypeDialog(QWidget *parent) :
    ChoiceDialog(parent)
{
    m_view = new DocumentTypeView;
    setTreeView(m_view);

    restoreDialogState();

    setWindowTitle(tr("Document types"));
    setWindowIcon(QIcon(":/icons/icons/doctype-16.png"));

    setInfoIconVisible();
    setInfoText(tr("Use color to highlight documents!"));
}

DocumentTypeDialog::~DocumentTypeDialog()
{
    saveDialogState();

    delete m_view;
}

void DocumentTypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("DocumentTypeDialog/geometry").toByteArray());
}

void DocumentTypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("DocumentTypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}
