#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QShortcut>

#include "utils/basecontextmenu.h"

class Table : public QTableWidget
{
    Q_OBJECT

    Q_PROPERTY(bool insertEnabled READ isInsertEnabled WRITE setInsertEnabled)
    Q_PROPERTY(bool editEnabled READ isEditEnabled WRITE setEditEnabled)
    Q_PROPERTY(bool removeEnabled READ isRemoveEnabled WRITE setRemoveEnabled)
    Q_PROPERTY(bool refreshEnabled READ isRefreshEnabled WRITE setRefreshEnabled)

    Q_PROPERTY(bool contextMenuEnabled READ isContextMenuEnabled WRITE setContextMenuEnabled)
    Q_PROPERTY(bool shortcutsEnabled READ isShortcutsEnabled WRITE setShortcutsEnabled)

public:
    Table(QWidget *parent = nullptr);
    ~Table();

    void setInsertEnabled(bool);
    void setEditEnabled(bool);
    void setRemoveEnabled(bool);
    void setRefreshEnabled(bool);
    void setContextMenuEnabled(bool);
    void setShortcutsEnabled(bool);

    bool isInsertEnabled() const { return m_insertEnabled; };
    bool isEditEnabled() const { return m_editEnabled; };
    bool isRemoveEnabled() const { return m_removeEnabled; };
    bool isRefreshEnabled() const { return m_refreshEnabled; };
    bool isContextMenuEnabled() const { return m_contextMenuEnabled; };
    bool isShortcutsEnabled() const { return m_shortcutsEnabled; };

private:
    QShortcut *m_insertShortcut;
    QShortcut *m_editShortcut;
    QShortcut *m_removeShortcut;
    QShortcut *m_refreshShortcut;

    void setupShortcuts();

    bool m_insertEnabled = true;
    bool m_editEnabled = true;
    bool m_removeEnabled = true;
    bool m_refreshEnabled = true;
    bool m_contextMenuEnabled = true;
    bool m_shortcutsEnabled = true;

private slots:
    void contextMenuRequested(const QPoint &pos);
    void onSelectionChanged();

protected:
    virtual void contextMenu(BaseContextMenu &) {};

signals:
    void onInsert();
    void onEdit(int row);
    void onRemove(const QList<QTableWidgetSelectionRange> &rangeList);
    void onRefresh(Qt::SortOrder);
};

#endif // TABLE_H
