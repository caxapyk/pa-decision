#ifndef SUBJECTTABLE_H
#define SUBJECTTABLE_H

#include "table.h"

class SubjectTable : public Table
{
    Q_OBJECT

public:
    SubjectTable(QWidget *parent = nullptr);

private:
    QStringList m_subjectHeaderLabels = {"Action", tr("ID"), tr("Type"), tr("Name"), tr("Address"), tr("Description")};

    void contextMenu(BaseContextMenu &menu) override;

    void insertRowPrivate();
    void removeRowPrivate();
};

#endif // SUBJECTTABLE_H
