#include "subjecttypedialog.h"
#include "application.h"

#include <QDebug>

SubjectTypeDialog::SubjectTypeDialog(QWidget *parent) :
    ChoiceDialog(parent)
{
    m_view = new SubjectTypeView;
    setTreeView(m_view);

    restoreDialogState();

    setWindowTitle(tr("Subjects"));
    setWindowIcon(QIcon(":/icons/icons/subject-16.png"));

    setInfoIconVisible(true);
    setInfoText(tr("Use subjects for grouping desicions!"));
}

SubjectTypeDialog::~SubjectTypeDialog()
{
    saveDialogState();

    delete m_view;
}

void SubjectTypeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("SubjectTypeDialog/geometry").toByteArray());
}

void SubjectTypeDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("SubjectTypeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();
}
