#include "documenttypetreeview.h"
#include "application.h"

#include <QHeaderView>

DocumentTypeTreeView::DocumentTypeTreeView(QWidget *parent) : ReferenceTreeView(parent)
{
    m_model = new DocumentTypeModel;
    m_model->select();

    _setModel(m_model);

    //setItemDelegateForColumn(2, new ColorPickerItemDelegate);
}

DocumentTypeTreeView::~DocumentTypeTreeView()
{
    saveViewState();

    delete m_model;
}

void DocumentTypeTreeView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/DocumentTypeTreeView").toByteArray());
}

void DocumentTypeTreeView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("DocumentTypeTreeView", header()->saveState());
    settings->endGroup();
}
