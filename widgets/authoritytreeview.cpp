#include "authoritytreeview.h"

#include <QDebug>

AuthorityTreeView::AuthorityTreeView(QWidget *parent) : CustomTreeView(parent)
{

}

void AuthorityTreeView::contextMenu(CustomContextMenu *menu)
{
    QModelIndex currentIndex = indexAt(viewport()->mapFromGlobal(QCursor().pos()));
    setCurrentIndex(currentIndex);

    QAction openInNTAction(tr("Open in new tab"));
    connect(&openInNTAction, &QAction::triggered, this, [=] {
            emit openInNewTabRequested(currentIndex);
    });
    openInNTAction.setEnabled(currentIndex.isValid() && currentIndex.parent().isValid());
    menu->insertAction(menu->action(CustomContextMenu::Insert), &openInNTAction);

    menu->insertSeparator(menu->action(CustomContextMenu::Insert));

    QAction detailsAction(tr("Details"));
    connect(&detailsAction, &QAction::triggered, this, [=] {
        emit detailsRequested();
    });
    detailsAction.setEnabled(currentIndex.isValid() && currentIndex.parent().isValid());
    menu->addAction(&detailsAction);

    qDebug() << "ghere";
}
