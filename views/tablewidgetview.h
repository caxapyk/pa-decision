#ifndef TABLEWIDGETVIEW_H
#define TABLEWIDGETVIEW_H

#include "widgets/basecontextmenu.h"

#include <QTableWidget>
#include <QShortcut>

class TableWidgetView : public QTableWidget
{
    Q_OBJECT
public:
    TableWidgetView(QWidget *parent = nullptr);
    ~TableWidgetView();

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

    QList<int> selectedRows() const;

public slots:
    virtual void _insertRow() {};
    virtual void editRow() {};
    virtual void removeRows() {};
    virtual void refresh() {};

private:
    QShortcut *m_insertShortcut;
    QShortcut *m_editShortcut;
    QShortcut *m_removeShortcut;
    QShortcut *m_refreshShortcut;

    void setupShortcuts();

    bool m_insertEnabled = true;
    bool m_editEnabled = false;
    bool m_removeEnabled = false;
    bool m_refreshEnabled = true;
    bool m_contextMenuEnabled = true;
    bool m_shortcutsEnabled = true;

private slots:
    void contextMenuRequested(const QPoint &pos);

protected slots:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

protected:
    void _clear();

    void virtual restoreViewState() {};
    void virtual saveViewState() {};

    virtual void contextMenu(BaseContextMenu &) {};
};

#endif // TABLEWIDGETVIEW_H
