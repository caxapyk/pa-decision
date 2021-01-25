#ifndef AUTHORITYTREEVIEW_H
#define AUTHORITYTREEVIEW_H

#include "widgets/customtreeview.h"
#include "utils/customcontextmenu.h"

class AuthorityTreeView : public CustomTreeView
{
    Q_OBJECT

public:
    AuthorityTreeView(QWidget *parent = nullptr);

private:
    void contextMenu(CustomContextMenu &menu) override;

signals:
    void openInNewTabRequested(const QModelIndex &index);
    void detailsRequested();
};

#endif // AUTHORITYTREEVIEW_H
