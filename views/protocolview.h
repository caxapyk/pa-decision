#ifndef PROTOCOLVIEW_H
#define PROTOCOLVIEW_H

#include "treeview.h"
#include "models/protocolmodel.h"

#include <QSortFilterProxyModel>

class ProtocolView : public TreeView
{
    Q_OBJECT
public:
    ProtocolView(QWidget *parent = nullptr);
    ~ProtocolView();

    void insertRow(const QModelIndex &index) override;
    void removeRow(const QModelIndex &index) override;
    void refresh() override;

    ProtocolModel *model() { return m_model; };
    QSortFilterProxyModel *proxyModel() { return m_proxyModel; };

public slots:
    void details();

private:
    ProtocolModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    void restoreViewState() override;
    void saveViewState() override;
};

#endif // PROTOCOLVIEW_H
