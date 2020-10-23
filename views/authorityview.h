#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "models/authoritytreemodel.h"
#include "views/view.h"

#include <QAction>
#include <QIcon>
#include <QShortcut>
#include <QSortFilterProxyModel>

namespace Ui {
class AuthorityView;
}

class AuthorityView : public View
{
    Q_OBJECT

public:
    explicit AuthorityView(QWidget *parent = nullptr);
    ~AuthorityView();

private:
    Ui::AuthorityView *ui;

    AuthorityTreeModel *m_authorityModel;

    QShortcut *openShortcut;
    QShortcut *refreshShortcut;
    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;

    void initialize() override;

    void contextMenu();
    void setupShortcuts();

private slots:
    void clicked(const QModelIndex &index);
    void open();
    void refresh();
    void insert();
    void edit();
    void remove();
    void info();
};

#endif // AUTHORITYVIEW_H
