#ifndef REFERENCETREEVIEW_H
#define REFERENCETREEVIEW_H

#include "treeview.h"

#include <QSqlTableModel>
#include <QSortFilterProxyModel>

class ReferenceTreeView : public TreeView
{
    Q_OBJECT
public:
    ReferenceTreeView(QWidget *parent = nullptr);
    ~ReferenceTreeView();

    void _setModel(QSqlTableModel *model);

protected slots:
    virtual void restoreViewState() override;
    virtual void saveViewState() override;

private:
    QSortFilterProxyModel *m_proxyModel;

private slots:
    void insertRow(const QModelIndex &index) override;
    void removeRow(const QModelIndex &index) override;
    void refresh() override;
};

#endif // REFERENCETREEVIEW_H
