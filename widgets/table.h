#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QShortcut>

#include "utils/basecontextmenu.h"

class Table : public QTableWidget
{
    Q_OBJECT
public:
    Table(QWidget *parent = nullptr);
    ~Table();

    void setInsertEnabled(bool ok);
    void setEditEnabled(bool ok);
    void setRemoveEnabled(bool ok);
    void setRefreshEnabled(bool ok);

    bool isInsertEnabled() const;
    bool isEditEnabled() const;
    bool isRemoveEnabled() const;
    bool isRefreshEnabled() const;

private:
    QShortcut *m_insertShortcut;
    QShortcut *m_editShortcut;
    QShortcut *m_removeShortcut;
    QShortcut *m_refreshShortcut;

    void setupShortcuts();

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
