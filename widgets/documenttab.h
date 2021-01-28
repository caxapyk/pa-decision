#ifndef DECISIONTAB_H
#define DECISIONTAB_H

#include "tab.h"
#include "views/documentview.h"

class DocumentTab : public Tab
{
    Q_OBJECT
public:
    DocumentTab(const QVariant &authorityId, QWidget *parent = nullptr);
    ~DocumentTab();

    DocumentView *view() { return m_view; };

private:
    DocumentView *m_view;
};

#endif // DECISIONTAB_H
