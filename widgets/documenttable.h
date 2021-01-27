#ifndef DECISIONTABLE_H
#define DECISIONTABLE_H

#include "table.h"

class DocumentTable : public Table
{
    Q_OBJECT
public:
    DocumentTable();

    void clear();

private:
    QStringList m_headerLabels = {tr("ID"), tr("Number"), tr("Name"), tr("Comment")};

    void contextMenu(BaseContextMenu &menu) override;
};

#endif // DECISIONTABLE_H
