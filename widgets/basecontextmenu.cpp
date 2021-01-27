#include "basecontextmenu.h"

#include <QItemSelectionModel>
#include <QtDebug>

BaseContextMenu::BaseContextMenu(BaseContextMenu::StandardActions actions, QWidget *parent) : QMenu(parent)
{
    // add action
    if(actions & BaseContextMenu::Insert || actions & BaseContextMenu::All) {
        action_insert = addAction(tr("New"));
        action_insert->setIcon(QIcon(":/icons/icons/add-16.png"));
        connect(action_insert, &QAction::triggered, this, [=] { emit insertRequested(); });
    }

    // edit action
    if(actions & BaseContextMenu::Edit || actions & BaseContextMenu::All) {
        action_edit = addAction(tr("Edit"));
        action_edit->setIcon(QIcon(":/icons/icons/edit-16.png"));
        connect(action_edit, &QAction::triggered, this, [=] { emit editRequested(); });
    }

    // remove action
    if(actions & BaseContextMenu::Remove || actions & BaseContextMenu::All) {
        action_remove = addAction(tr("Remove"));
        action_remove->setIcon(QIcon(":/icons/icons/remove-16.png"));
        connect(action_remove, &QAction::triggered, this, [=] { emit removeRequested(); });
    }

    addSeparator();

    // refresh action
    if(actions & BaseContextMenu::Refresh || actions & BaseContextMenu::All) {
        action_refresh = addAction(tr("Refresh"));
        action_refresh->setIcon(QIcon(":/icons/icons/refresh-16.png"));
        connect(action_refresh, &QAction::triggered, this, [=] { emit refreshRequested(); });
    }
}

BaseContextMenu::~BaseContextMenu()
{
    delete action_insert;
    delete action_edit;
    delete action_remove;
    delete action_refresh;
}

QAction *BaseContextMenu::action(BaseContextMenu::StandardAction which) const
{
    switch (which) {
    case BaseContextMenu::Insert:
        return action_insert;
        break;
    case BaseContextMenu::Edit:
        return action_edit;
        break;
    case BaseContextMenu::Remove:
        return action_remove;
        break;
    case BaseContextMenu::Refresh:
        return action_refresh;
        break;
    default:
        return nullptr;
    }
}

void BaseContextMenu::setEnabled(bool b, BaseContextMenu::StandardAction which) {
    if(action(which)) {
        action(which)->setEnabled(b);
    }
}
