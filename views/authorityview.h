#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "treeview.h"
#include "models/authoritymodel.h"
#include "models/authoritysortmodel.h"

class AuthorityView : public TreeView
{
    Q_OBJECT

public:
    AuthorityView(QWidget *parent = nullptr);
    ~AuthorityView();

    QVariant id() const { return m_authorityId; };

public slots:
    void insertRow() override;
    void removeRows() override;
    void refresh() override;

private:
    QAction *openInNTAction;
    QAction *detailsAction;

    AuthorityModel *m_model;
    AuthoritySortModel *m_proxyModel;

    QVariant m_authorityId;

    void initialize();

    void contextMenu(BaseContextMenu &menu) override;
    void restoreViewState() override;
    void saveViewState() override;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    void details();

signals:
    void openInNewTabRequested(const QVariant &id);
};

#endif // AUTHORITYVIEW_H
