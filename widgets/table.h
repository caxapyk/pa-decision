#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QShortcut>

#include "utils/customcontextmenu.h"

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

private:
    QShortcut *m_insertShortcut;
    QShortcut *m_editShortcut;
    QShortcut *m_removeShortcut;
    QShortcut *m_refreshShortcut;

    void setupShortcuts();

private slots:
    void contextMenuRequested(const QPoint &pos);

protected:
    virtual void contextMenu(CustomContextMenu &menu) {};

signals:
    void onInsert(const QModelIndex &index);
    void onEdit(const QModelIndex &index);
    void onRemove(const QModelIndex &index);
    void onRefresh();
};

#endif // TABLE_H
