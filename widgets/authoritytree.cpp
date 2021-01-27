#include "authoritytree.h"

#include <QDebug>

AuthorityTree::AuthorityTree(QWidget *parent) : CustomTreeView(parent)
{
    openInNTAction = new QAction(tr("Open in new tab"));
    detailsAction = new QAction(tr("Details"));

    connect(openInNTAction, &QAction::triggered, this, [=] {
            emit openInNewTabRequested(currentIndex());
    });

    connect(detailsAction, &QAction::triggered, this, [=] {
        emit detailsRequested();
    });
}

AuthorityTree::~AuthorityTree()
{
    delete openInNTAction;
    delete detailsAction;
}

void AuthorityTree::contextMenu(BaseContextMenu &menu)
{
    QModelIndex index = currentIndex();

    openInNTAction->setEnabled(index.isValid() && index.parent().isValid());
    menu.insertAction(menu.action(BaseContextMenu::Insert), openInNTAction);

    menu.insertSeparator(menu.action(BaseContextMenu::Insert));

    detailsAction->setEnabled(index.isValid() && index.parent().isValid());
    menu.addAction(detailsAction);
}
