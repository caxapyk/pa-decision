#ifndef AUTHORITYTREEVIEW_H
#define AUTHORITYTREEVIEW_H

#include "widgets/customtreeview.h"

class AuthorityTree : public CustomTreeView
{
    Q_OBJECT

public:
    AuthorityTree(QWidget *parent = nullptr);
    ~AuthorityTree();

private:
    void contextMenu(BaseContextMenu &menu) override;

    QAction *openInNTAction;
    QAction *detailsAction;

signals:
    void openInNewTabRequested(const QModelIndex &index);
    void detailsRequested();
};

#endif // AUTHORITYTREEVIEW_H
