#include "documenttypeview.h"
#include "application.h"

#include <QHeaderView>

DocumentTypeView::DocumentTypeView(QWidget *parent) : ReferenceView(parent)
{
    m_model = new DocumentTypeModel;
    m_model->select();

    _setModel(m_model);

    //setItemDelegateForColumn(2, new ColorPickerItemDelegate);

    restoreViewState();
}

DocumentTypeView::~DocumentTypeView()
{
    saveViewState();

    delete m_model;
}

void DocumentTypeView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/DocumentTypeView").toByteArray());
}

void DocumentTypeView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("DocumentTypeView", header()->saveState());
    settings->endGroup();
}
