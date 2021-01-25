#include "decisiontable.h"

#include <QDebug>
#include <QSizePolicy>

DecisionTable::DecisionTable()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setSelectionBehavior(Table::SelectRows);
    setSelectionMode(Table::ExtendedSelection);
}

void DecisionTable::contextMenu(CustomContextMenu &menu)
{
    //menu.addAction("Open");
}
