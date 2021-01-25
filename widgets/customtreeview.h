#ifndef TREECONTROL_H
#define TREECONTROL_H

#include "utils/customcontextmenu.h"

#include <QTreeView>
#include <QShortcut>

class CustomTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CustomTreeView(QWidget *parent = nullptr);
    ~CustomTreeView();

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
    virtual void contextMenu(CustomContextMenu *) {};

signals:
    void onInsert(const QModelIndex &index);
    void onEdit(const QModelIndex &index);
    void onRemove(const QModelIndex &index);
    void onRefresh();
};

#endif // TREECONTROL_H
