#include "subjtypedialog.h"
#include "application.h"

#include <QDebug>

SubjtypeDialog::SubjtypeDialog(QWidget *parent) :
    ChoiceDialog(parent)
{
    m_view = new SubjectTypeTreeView;
    setTreeView(m_view);

    restoreDialogState();

    setWindowTitle(tr("Subjects"));
    setWindowIcon(QIcon(":/icons/icons/subject-16.png"));

    setInfoIconVisible(true);
    setInfoText(tr("Use subjects for grouping desicions!"));
}

SubjtypeDialog::~SubjtypeDialog()
{
    saveDialogState();

    delete m_view;
}

void SubjtypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("SubjtypeDialog/geometry").toByteArray());
}

void SubjtypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("SubjtypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}
