#ifndef DOCUMENTTYPETREEVIEW_H
#define DOCUMENTTYPETREEVIEW_H

#include "referencetreeview.h"
#include "models/documenttypemodel.h"

class DocumentTypeTreeView : public ReferenceTreeView
{
    Q_OBJECT
public:
    DocumentTypeTreeView(QWidget *parent = nullptr);
    ~DocumentTypeTreeView();

private:
    DocumentTypeModel *m_model;

private slots:
    void restoreViewState() override;
    void saveViewState() override;
};

#endif // DOCUMENTTYPETREEVIEW_H
