#ifndef SUBJECTTYPEVIEW_H
#define SUBJECTTYPEVIEW_H

#include "referenceview.h"
#include "models/subjecttypemodel.h"

class SubjectTypeView : public ReferenceView
{
public:
    SubjectTypeView(QWidget *parent = nullptr);
    ~SubjectTypeView();

private:
    SubjectTypeModel *m_model;

    void restoreViewState() override;
    void saveViewState() override;
};

#endif // SUBJECTTYPEVIEW_H
