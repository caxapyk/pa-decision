#include "subjecttypeview.h"
#include "application.h"

#include <QHeaderView>

SubjectTypeView::SubjectTypeView(QWidget *parent) : ReferenceView(parent)
{
    m_model = new SubjectTypeModel;
    m_model->select();

    _setModel(m_model);

    restoreViewState();
}

SubjectTypeView::~SubjectTypeView()
{
    saveViewState();

    delete m_model;
}

void SubjectTypeView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/SubjectTypeView").toByteArray());
}

void SubjectTypeView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("SubjectTypeView", header()->saveState());
    settings->endGroup();
}
