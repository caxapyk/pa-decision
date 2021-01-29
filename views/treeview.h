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

    void setInsertEnabled(bool);
    bool isInsertEnabled() const { return m_insertEnabled; };

    void setEditEnabled(bool);
    bool isEditEnabled() const { return m_editEnabled; };

    void setRemoveEnabled(bool);
    bool isRemoveEnabled() const { return m_removeEnabled; };

    void setRefreshEnabled(bool);
    bool isRefreshEnabled() const { return m_refreshEnabled; };

    void setContextMenuEnabled(bool);
    bool isContextMenuEnabled() const { return m_contextMenuEnabled; };

    void setShortcutsEnabled(bool);
    bool isShortcutsEnabled() const { return m_shortcutsEnabled; };

public slots:
    virtual void insertRow() {};
    virtual void editRow();
    virtual void removeRows() {};
    virtual void refresh() {};

private:
    QShortcut *m_insertShortcut;
    QShortcut *m_editShortcut;
    QShortcut *m_removeShortcut;
    QShortcut *m_refreshShortcut;

    bool m_insertEnabled = true;
    bool m_editEnabled = false;
    bool m_removeEnabled = false;
    bool m_refreshEnabled = true;
    bool m_contextMenuEnabled = true;
    bool m_shortcutsEnabled = true;

    void setupShortcuts();

private slots:
    void contextMenuRequested(const QPoint &pos);

protected slots:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

protected:
    void virtual restoreViewState() {};
    void virtual saveViewState() {};

    virtual void contextMenu(BaseContextMenu &) {};
};

#endif // TREEVIEW_H
