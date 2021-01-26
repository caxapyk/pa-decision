#ifndef CUSTOMCONTEXMENU_H
#define CUSTOMCONTEXMENU_H

#include <QObject>
#include <QMenu>
#include <QModelIndexList>

class BaseContextMenu : public QMenu
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

    BaseContextMenu(BaseContextMenu::StandardActions actions, QWidget *parent = nullptr);
    ~BaseContextMenu();
    QAction *action(BaseContextMenu::StandardAction which) const;
    void setEnabled(bool b, BaseContextMenu::StandardAction which);
    //void setSelection(const QModelIndexList &selected);

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

Q_DECLARE_OPERATORS_FOR_FLAGS(BaseContextMenu::StandardActions)

#endif // CUSTOMCONTEXMENU_H
