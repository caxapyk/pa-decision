#ifndef DECISIONTABLE_H
#define DECISIONTABLE_H

#include "table.h"

class DecisionTable : public Table
{
    Q_OBJECT
public:
    DecisionTable();

private:
    void contextMenu(CustomContextMenu &menu) override;
};

#endif // DECISIONTABLE_H
