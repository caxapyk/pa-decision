#ifndef SUBJECTTYPETREEVIEW_H
#define SUBJECTTYPETREEVIEW_H

#include "referencetreeview.h"
#include "models/subjtypemodel.h"

class SubjectTypeTreeView : public ReferenceTreeView
{
public:
    SubjectTypeTreeView(QWidget *parent = nullptr);
    ~SubjectTypeTreeView();

private:
    SubjtypeModel *m_model;

private slots:
    void restoreViewState() override;
    void saveViewState() override;
};

#endif // SUBJECTTYPETREEVIEW_H
