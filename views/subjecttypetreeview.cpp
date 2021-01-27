#include "subjecttypetreeview.h"
#include "application.h"

#include <QHeaderView>

SubjectTypeTreeView::SubjectTypeTreeView(QWidget *parent) : ReferenceTreeView(parent)
{
    m_model = new SubjtypeModel;
    m_model->select();

    _setModel(m_model);
}

SubjectTypeTreeView::~SubjectTypeTreeView()
{
    saveViewState();

    delete m_model;
}

void SubjectTypeTreeView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/SubjectTypeTreeView").toByteArray());
}

void SubjectTypeTreeView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("SubjectTypeTreeView", header()->saveState());
    settings->endGroup();
}
