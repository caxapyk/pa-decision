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


    AFTreeModel *model() { return m_model; };
    QSortFilterProxyModel *proxyModel() { return m_proxyModel; };

public slots:
    void insertRow() override;
    void removeRows() override;
    void refresh() override;

    void details();
    void protocols();

private:
    AFTreeModel *m_model;
    AFSortModel *m_proxyModel;

    void contextMenu(BaseContextMenu &menu) override;
    void restoreViewState() override;
    void saveViewState() override;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
};

#endif // AFVIEW_H
