#ifndef CUSTOMCONTEXMENU_H
#define CUSTOMCONTEXMENU_H

#include <QObject>
#include <QMenu>
#include <QModelIndexList>

class CustomContextMenu : public QMenu
{
    Q_OBJECT
public:
    enum StandardAction {
        Insert              = 0x01,
        Edit                = 0x02,
        Remove              = 0x04,
        Refresh             = 0x08,
        NoStandartAction    = 0x10,
        All                 = 0x20,
    };

    Q_DECLARE_FLAGS(StandardActions, StandardAction)
    Q_FLAG(StandardActions)

    CustomContextMenu(CustomContextMenu::StandardActions actions, QWidget *parent = nullptr);
    ~CustomContextMenu();
    QAction *action(CustomContextMenu::StandardAction which) const;
    void setEnabled(bool b, CustomContextMenu::StandardAction which);
    void setSelection(const QModelIndexList &selected);

signals:
    void insertRequested();
    void editRequested();
    void removeRequested();
    void refreshRequested();

private:
    QAction *action_insert = nullptr;
    QAction *action_edit = nullptr;
    QAction *action_remove = nullptr;
    QAction *action_refresh = nullptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CustomContextMenu::StandardActions)

#endif // CUSTOMCONTEXMENU_H
