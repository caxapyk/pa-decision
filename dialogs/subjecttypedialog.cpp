#include "subjecttypedialog.h"
#include "application.h"

#include <QDebug>

SubjectTypeDialog::SubjectTypeDialog(QWidget *parent) :
    ReferenceDialog(parent)
{
    m_view = new SubjectTypeView;
    _layout()->insertWidget(0, m_view);

    restoreDialogState();

    setWindowTitle(tr("Subjects"));
    setWindowIcon(QIcon(":/icons/icons/subject-16.png"));
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
