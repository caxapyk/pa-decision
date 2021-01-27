#ifndef AUTHORITYTREEVIEW_H
#define AUTHORITYTREEVIEW_H

#include "treeview.h"

#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"

class AuthorityTreeView : public TreeView
{
    Q_OBJECT

public:
    AuthorityTreeView(QWidget *parent = nullptr);
    ~AuthorityTreeView();

    QVariant id() const { return m_authorityId; };

private:
    void contextMenu(BaseContextMenu &menu) override;

    QAction *openInNTAction;
    QAction *detailsAction;

    AuthorityModel *m_authorityModel;
    AuthorityProxyModel *m_authorityProxyModel;

    QVariant m_authorityId;

    void initialize();

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    void restoreViewState() override;
    void saveViewState() override;

    void insertRow(const QModelIndex &index) override;
    void removeRow(const QModelIndex &index) override;
    void refresh() override;

    void details();

signals:
    void openInNewTabRequested(const QModelIndex &index);
};

#endif // AUTHORITYTREEVIEW_H
