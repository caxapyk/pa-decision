#include "customcontextmenu.h"

#include <QItemSelectionModel>
#include <QtDebug>

CustomContextMenu::CustomContextMenu(CustomContextMenu::StandardActions actions, QWidget *parent) : QMenu(parent)
{
    // add action
    if(actions & CustomContextMenu::Insert || actions & CustomContextMenu::All) {
        action_insert = addAction(tr("New"));
        action_insert->setIcon(QIcon(":/icons/icons/add-16.png"));
        connect(action_insert, &QAction::triggered, this, [=] { emit insertRequested(); });
    }

    // edit action
    if(actions & CustomContextMenu::Edit || actions & CustomContextMenu::All) {
        action_edit = addAction(tr("Edit"));
        action_edit->setIcon(QIcon(":/icons/icons/edit-16.png"));
        connect(action_edit, &QAction::triggered, this, [=] { emit editRequested(); });
    }

    // remove action
    if(actions & CustomContextMenu::Remove || actions & CustomContextMenu::All) {
        action_remove = addAction(tr("Remove"));
        action_remove->setIcon(QIcon(":/icons/icons/remove-16.png"));
        connect(action_remove, &QAction::triggered, this, [=] { emit removeRequested(); });
    }

    addSeparator();

    // refresh action
    if(actions & CustomContextMenu::Refresh || actions & CustomContextMenu::All) {
        action_refresh = addAction(tr("Refresh"));
        action_refresh->setIcon(QIcon(":/icons/icons/refresh-16.png"));
        connect(action_refresh, &QAction::triggered, this, [=] { emit refreshRequested(); });
    }
}

CustomContextMenu::~CustomContextMenu()
{
    delete action_insert;
    delete action_edit;
    delete action_remove;
    delete action_refresh;
}

QAction *CustomContextMenu::action(CustomContextMenu::StandardAction which) const
{
    switch (which) {
    case CustomContextMenu::Insert:
        return action_insert;
        break;
    case CustomContextMenu::Edit:
        return action_edit;
        break;
    case CustomContextMenu::Remove:
        return action_remove;
        break;
    case CustomContextMenu::Refresh:
        return action_refresh;
        break;
    default:
        return nullptr;
    }
}

void CustomContextMenu::setSelection(const QModelIndexList &selected)
{
    //disable edit action for more then one row selected
    setEnabled(selected.length() == 1, CustomContextMenu::Edit);

    setEnabled(!selected.isEmpty(), CustomContextMenu::Remove);
}

void CustomContextMenu::setEnabled(bool b, CustomContextMenu::StandardAction which) {
    if(action(which)) {
        action(which)->setEnabled(b);
    }
}
