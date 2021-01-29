#ifndef REFERENCEVIEW_H
#define REFERENCEVIEW_H

#include "treeview.h"

#include <QSqlTableModel>
#include <QSortFilterProxyModel>

class ReferenceView : public TreeView
{
    Q_OBJECT
public:
    ReferenceView(QWidget *parent = nullptr);
    ~ReferenceView();

    void _setModel(QSqlTableModel *model);

    void insertRow() override;
    void removeRows() override;
    void refresh() override;

protected slots:
    virtual void restoreViewState() override {};
    virtual void saveViewState() override {};

private:
    QSortFilterProxyModel *m_proxyModel;
};

#endif // REFERENCEVIEW_H
