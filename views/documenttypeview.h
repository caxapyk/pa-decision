#ifndef DOCUMENTTYPEVIEW_H
#define DOCUMENTTYPEVIEW_H

#include "referenceview.h"
#include "models/documenttypemodel.h"

class DocumentTypeView : public ReferenceView
{
    Q_OBJECT
public:
    DocumentTypeView(QWidget *parent = nullptr);
    ~DocumentTypeView();

private:
    DocumentTypeModel *m_model;

    void restoreViewState() override;
    void saveViewState() override;
};

#endif // DOCUMENTTYPEVIEW_H
