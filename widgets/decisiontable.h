#ifndef DECISIONTABLE_H
#define DECISIONTABLE_H

#include "table.h"

class DecisionTable : public Table
{
    Q_OBJECT
public:
    DecisionTable();

    void refresh();
    void setAuthorityId(const QVariant &id) { m_authorityId = id; };
    QVariant authorityId() const { return m_authorityId; };

private:
    QVariant m_authorityId;
    QStringList m_headerLabels = {tr("ID"), tr("Number"), tr("Name"), tr("Comment")};
};

#endif // DECISIONTABLE_H
