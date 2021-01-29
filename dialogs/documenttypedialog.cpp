#include "documenttypedialog.h"

#include "application.h"
#include "models/colorpickeritemdelegate.h"

#include <QDebug>
#include <QHeaderView>

DocumentTypeDialog::DocumentTypeDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    m_view = new DocumentTypeView;
    _layout()->insertWidget(0, m_view);

    restoreDialogState();

    setWindowTitle(tr("Document types"));
    setWindowIcon(QIcon(":/icons/icons/doctype-16.png"));
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
