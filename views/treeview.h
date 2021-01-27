#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "widgets/basecontextmenu.h"

#include <QTreeView>
#include <QShortcut>

class TreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit TreeView(QWidget *parent = nullptr);
    ~TreeView();

    void setInsertEnabled(bool ok);
    void setEditEnabled(bool ok);
    void setRemoveEnabled(bool ok);
    void setRefreshEnabled(bool ok);

private:
    QShortcut *m_insertShortcut;
    QShortcut *m_editShortcut;
    QShortcut *m_removeShortcut;
    QShortcut *m_refreshShortcut;

    void setupShortcuts();

private slots:
    void contextMenuRequested(const QPoint &pos);

protected:
    virtual void insertRow(const QModelIndex &) {};
    virtual void editRow(const QModelIndex &index);
    virtual void removeRow(const QModelIndex &) {};
    virtual void refresh() {};

protected:
    virtual void contextMenu(BaseContextMenu &) {};

    void virtual restoreViewState(){};
    void virtual saveViewState(){};
};

#endif // TREEVIEW_H
