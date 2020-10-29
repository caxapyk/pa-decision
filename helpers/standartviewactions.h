#ifndef STANDARTVIEWSHORTCUTS_H
#define STANDARTVIEWSHORTCUTS_H

#include "dialogs/referencedialoginterface.h"

#include <QAbstractItemView>
#include <QObject>
#include <QShortcut>

class StandartViewActions : public QObject
{
    Q_OBJECT
public:
    explicit StandartViewActions(QObject *parent = nullptr);
    ~StandartViewActions();

    void setView(QAbstractItemView *view) { m_view = view; };
    void setCrudWidget(ReferenceDialogInterface *dialog) { m_dialog = dialog; };
    void bindShortcuts();

    QShortcut* insertShortcut() { return m_insertShortcut; };
    QShortcut* editShortcut() { return m_editShortcut; };
    QShortcut* removeShortcut() { return m_removeShortcut; };
    QShortcut* refreshShortcut() { return m_refreshShortcut; };

private:
    QShortcut *m_insertShortcut = nullptr;
    QShortcut *m_editShortcut = nullptr;
    QShortcut *m_removeShortcut = nullptr;
    QShortcut *m_refreshShortcut = nullptr;

    QAbstractItemView *m_view;
    ReferenceDialogInterface *m_dialog;
};

#endif // STANDARTVIEWSHORTCUTS_H
