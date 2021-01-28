#ifndef AFVIEW_H
#define AFVIEW_H

#include "treeview.h"
#include "models/aftreemodel.h"
#include "models/afsortmodel.h"

class AFView : public TreeView
{
    Q_OBJECT
public:
    AFView(QWidget *parent = nullptr);
    ~AFView();

    void insertRow(const QModelIndex &index) override;
    void removeRow(const QModelIndex &index) override;
    void refresh() override;

    AFTreeModel *model() { return m_model; };
    QSortFilterProxyModel *proxyModel() { return m_proxyModel; };

private:
    AFTreeModel *m_model;
    AFSortModel *m_proxyModel;

    void contextMenu(BaseContextMenu &menu) override;
    void restoreViewState() override;
    void saveViewState() override;

public slots:
    void details();
    void protocols();

private slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // AFVIEW_H
